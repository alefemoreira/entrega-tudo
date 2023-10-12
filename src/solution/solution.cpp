#include "solution.h"
#include "../reader/reader.h"
#include <iostream>
#include <vector>

using namespace std;

void Solution::build()
{
  this->cost = 0;
  int n = Reader::instance->getDimension();
  int k = Reader::instance->getMaxVehiclesQuantity();
  int l = Reader::instance->getMinimumDelivery();

  int maxVehicles = k;
  int maxVehicleCapacity = Reader::instance->getCarCapacity();
  int visited = 0;

  // Pontos não visitados
  vector<int> not_visited;
  for (int i = 1; i <= n; i++)
  {
    not_visited.push_back(i);
  }

  for (int i = 0; i < k; i++)
  {
    this->sequence[i].push_back(0);
  }

  while (!not_visited.empty())
  {
    int better_sequence = 0;
    int better_demand = 0;
    int better_point = 0;
    int better_index = 0;
    int better_cost = INFINITE;

    for (int i = 0; i < not_visited.size(); i++)
    {
      int vi = not_visited[i];
      for (int j = 0; j < k; j++) // veiculos
      {
        double cost = this->getCost(vi, j);

        if (cost < better_cost)
        {
          better_cost = cost;
          better_point = vi;
          better_index = i;
          better_demand = Reader::instance->getDemand(vi);
          better_sequence = j;
        }
      }
    }

    // Atualizando a solução
    this->sequence[better_sequence].push_back(better_point);
    not_visited.erase(not_visited.begin() + better_index);
    visited += 1;

    // cout << "---" << better_cost << " " << better_point << endl;

    this->cost += better_cost;
    if (better_sequence < maxVehicles)
      this->capacities[better_sequence] += better_demand;

    if (visited >= l && k <= maxVehicles)
    {
      k++;
    }
  }

  for (int i = 0; i < maxVehicles; i++)
  {
    int back = this->sequence[i].back();
    this->cost += Reader::instance->getDistance(back, 0);
    this->sequence[i].push_back(0);
  }
}

Solution::Solution(/* args */)
{
  int k = Reader::instance->getMaxVehiclesQuantity();
  int n = Reader::instance->getDimension();
  this->cost = 0;
  this->vehicles = 0;
  this->sequence = vector<vector<int>>(k + 1, vector<int>());
  this->capacities = vector<int>(k, 0);
}

Solution::~Solution() {}

void Solution::vnd()
{
  bool improved = false;

  do
  {
    improved = this->bestImprovementSwapVehicles();
  } while (improved);
}

bool Solution::bestImprovementSwapVehicles()
{
  int bestI, bestJ, bestK1, bestK2;
  int bestDelta = 0;
  int vehicles = Reader::instance->getMaxVehiclesQuantity();
  double r = Reader::instance->getCarUseCost();
  int Q = Reader::instance->getCarCapacity();
  // int dimension = this->

  for (int k1 = 0; k1 < vehicles; k1++)
  {
    for (int k2 = k1 + 1; k2 < vehicles; k2++) // k2 = k1 poderia fazer trocas na mesma rota
    {
      // cout << "Troca entre veiculo " << k1 + 1 << " e veiculo " << k2 + 1 << endl;
      for (int i = 1; i < this->sequence[k1].size() - 1; i++)
      {
        // só executa operações com o primeiro e o ultimo elemento quando for a rota dos terceirizados
        // if (k2 != vehicles && (i == 0 || i == this->sequence[k1].size() - 1))
        //   continue;

        int vi = this->sequence[k1][i];
        int viPrev = this->sequence[k1][i - 1];
        int viNext = this->sequence[k1][i + 1];

        // cout << viPrev  << " " << vi<< " " <<viNext ;

        int j = 1;

        for (; j < this->sequence[k2].size() - 1; j++)
        {
          // só executa operações com o primeiro e o ultimo elemento quando for a rota dos terceirizados
          // if (k2 != vehicles && (j == 0 || j == this->sequence[k2].size() - 1))
          //   continue;

          int vj = this->sequence[k2][j];
          int vjPrev = this->sequence[k2][j - 1];
          int vjNext = this->sequence[k2][j + 1];

          double delta = (Reader::instance->getDistance(vi, viPrev) + Reader::instance->getDistance(vi, viNext) +
                          Reader::instance->getDistance(vj, vjPrev) + Reader::instance->getDistance(vj, vjNext)) -
                         (Reader::instance->getDistance(vj, viPrev) + Reader::instance->getDistance(vj, viNext) -
                          Reader::instance->getDistance(vi, vjPrev) + Reader::instance->getDistance(vi, vjNext));
          ;

          // cout << " -- " << vjPrev  << " " << vj<< " " << vjNext << endl;

          // cout << " -> Delta: " << delta << " " << vi << " <-> " << vj << endl;

          // if (this->sequence[k2].size() == 2)
          //   delta += r;

          // if (this->sequence[k1].size() == 3)
          //   delta -= r;

          // Só atualiza os valores se a troca poder ser feita sem ultrapassar as capacidades dos carros
          if (delta < bestDelta)
          {
            int demand1 = Reader::instance->getDemand(this->sequence[k1][i]);
            int demand2 = Reader::instance->getDemand(this->sequence[k2][j]);
            int maxVehicleCapacity = Reader::instance->getCarCapacity();
            bool isAproved = (((this->capacities[k1] - demand1) + demand2) <= maxVehicleCapacity) && (((this->capacities[k2] - demand2) + demand1) <= maxVehicleCapacity);
            if (isAproved)
            {
              bestDelta = delta;
              bestI = i;
              bestJ = j;
              bestK1 = k1;
              bestK2 = k2;
            }
          }
        }
        // cout << endl;
      }
      // cout << endl;
    }
  }

  // cout << "Melhor delta entre os carros " << bestK1 << " e " << bestK2 << ": " << bestDelta << " " << this->sequence[bestK1][bestI] << " <-> " << this->sequence[bestK2][bestJ] << endl;

  if (bestDelta < 0)
  {
    // Verificando as capacidades dos veiculos
    int demand1 = Reader::instance->getDemand(this->sequence[bestK1][bestI]);
    int demand2 = Reader::instance->getDemand(this->sequence[bestK2][bestJ]);
    int maxVehicleCapacity = Reader::instance->getCarCapacity();
    bool isAproved = (((this->capacities[bestK1] - demand1) + demand2) <= maxVehicleCapacity) && (((this->capacities[bestK2] - demand2) + demand1) <= maxVehicleCapacity);
    // cout << "Capacidade carro k1: " << ((this->capacities[bestK1] - demand1) + demand2) << " - Capacidade antes da troca: " << this->capacities[bestK1] << endl;
    // cout << "Capacidade carro k2: " << ((this->capacities[bestK2] - demand2) + demand1) << " - Capacidade antes da troca: " << this->capacities[bestK2] << endl;
    // cout << "Capacidade total: " << maxVehicleCapacity << endl;
    // cout << "As Capacidades dos carros suportam as trocas: " << isAproved << endl;
    if (isAproved)
    {
      this->cost += bestDelta;
      this->capacities[bestK1] = this->capacities[bestK1] - demand1 + demand2;
      this->capacities[bestK2] = this->capacities[bestK2] - demand2 + demand1;
      this->swapElementsBetweenArrays(bestK1, bestI, bestK2, bestJ);
      return true;
    }
  }
  return false;
}

bool Solution::bestImprovementReinsertionVehicles()
{
  int bestI, bestJ, bestK1, bestK2;
  int bestDelta = 0;
  int vehicles = Reader::instance->getMaxVehiclesQuantity();
  double r = Reader::instance->getCarUseCost();
  int Q = Reader::instance->getCarCapacity();
  // int dimension = this->

  for (int k1 = 0; k1 < vehicles; k1++)
  {
    for (int k2 = k1; k2 < vehicles; k2++)
    {
      for (int i = 1; i < this->sequence[k1].size() - 1; i++)
      {
        int vi = this->sequence[k1][i];
        int viPrev = this->sequence[k1][i - 1];
        int viNext = this->sequence[k1][i + 1];

        int j = 1;
        if (k1 == k2)
          j = i + 1;

        for (; j < this->sequence[k2].size() - 1; j++)
        {
          int vj = this->sequence[k2][j];
          int vjNext = this->sequence[k2][j + 1];

          if (Reader::instance->getDemand(vi) + this->capacities[k2] > Q)
            continue;

          double delta = Reader::instance->getDistance(vi, vjNext) + Reader::instance->getDistance(vj, vi) +
                         Reader::instance->getDistance(viPrev, viNext) -
                         Reader::instance->getDistance(viPrev, vi) - Reader::instance->getDistance(vi, viNext) -
                         Reader::instance->getDistance(vj, vjNext);

          if (this->sequence[k2].size() == 2)
            delta += r;

          if (this->sequence[k1].size() == 3)
            delta -= r;

          if (delta < bestDelta)
          {
            bestDelta = delta;
            bestI = i;
            bestJ = j;
            bestK1 = k1;
            bestK2 = k2;
          }
        }
      }
    }
  }
  if (bestDelta < 0)
  {
    this->cost += bestDelta;

    int demand = Reader::instance->getDemand(this->sequence[bestK1][bestI]);
    this->capacities[bestK1] -= demand;
    this->capacities[bestK2] += demand;
    this->reinsertion(bestK1, bestI, bestK2, bestJ);
    return true;
  }
  return false;
}

void Solution::swapElementsBetweenArrays(int k1, int i, int k2, int j)
{
  int temp = this->sequence[k1][i];
  this->sequence[k1][i] = this->sequence[k2][j];
  this->sequence[k2][j] = temp;
}

void Solution::reinsertion(int k1, int i, int k2, int j)
{
  int v = this->sequence[k1][i];
  auto k1Begin = this->sequence[k1].begin();
  auto k2Begin = this->sequence[k2].begin();
  this->sequence[k1].erase(k1Begin + i);
  this->sequence[k2].insert(k2Begin + j, v);
}

void Solution::setCost(double cost) { this->cost = cost; }

void Solution::setVehicles(int vehicles) { this->vehicles = vehicles; }

void Solution::setSequence(vector<vector<int>> *sequence)
{
  this->sequence = *sequence;
}

double Solution::getCost(int vi, int j)
{
  int maxVehicles = Reader::instance->getMaxVehiclesQuantity();

  if (j == maxVehicles)
  {
    double cost = Reader::instance->getOutsourcing(vi);
    return cost;
  }
  else
  {
    int demand = Reader::instance->getDemand(vi);
    int last = this->sequence[j].back();
    int maxDemand = Reader::instance->getCarCapacity();

    double r = Reader::instance->getCarUseCost();
    double cost = this->capacities[j] == 0 ? r : 0;
    cost += Reader::instance->getDistance(last, vi);

    if (this->capacities[j] + demand >= maxDemand)
    {
      return INFINITE;
    }

    return cost;
  }
}

void Solution::resume()
{
  cout << "Custo: " << this->cost << endl;
  cout << "Número de veiculos: " << this->vehicles << endl;
  for (size_t i = 0; i < this->sequence.size(); i++)
  {
    if (i != sequence.size() - 1)
    {
      cout << "Rota do veiculo " << i + 1 << ":" << endl;
    }
    else
    {
      cout << "Pontos de entrega tercerizados: " << endl;
    }

    for (size_t j = 0; j < this->sequence[i].size(); j++)
    {
      cout << " " << this->sequence[i][j] << " ";
    }
    cout << endl;
  }
}