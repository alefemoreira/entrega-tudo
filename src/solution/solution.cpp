#include "solution.h"
#include "../reader/reader.h"
#include <iostream>
#include <vector>

using namespace std;

void Solution::build()
{
  int n = Reader::instance->getDimension();
  int k = Reader::instance->getMaxVehiclesQuantity();

  vector<vector<int>> g(n, vector<int>(n));

  g = {
      {0, 4, 9, 3, 20},
      {4, 0, 2, 5, 18},
      {9, 2, 0, 7, 21},
      {3, 5, 7, 0, 1},
      {20, 18, 21, 1, 0},
  };

  int maxVehicles = Reader::instance->getMaxVehiclesQuantity();
  int maxVehicleCapacity = Reader::instance->getCarCapacity();

  // Pontos não visitados
  vector<int> not_visited;
  for (int i = 0; i < n; i++)
  {
    not_visited.push_back(i);
  }

  while (!not_visited.empty())
  {
    int better_vehicle = 0;
    int better_point = 0;
    int better_distance_by_capacity = 0;
    bool hasAvailibleVehicles = false;

    for (int i = 0; i < not_visited.size(); i++)
    {
      for (size_t j = 0; j < k; j++) // veiculos
      {
        // Confirmando que ainda há carros disponiveis
        hasAvailibleVehicles = true;

        int distance_by_capacity;
        int last_point = sequence[j].size() - 1; // Ultimo ponto de uma rota

        // Verificando se o carro suporta a próxima entrega
        if (this->capacities[j] + g[last_point][i] < maxVehicleCapacity)
        {
          if (this->capacities[j] > 0)
          {
            distance_by_capacity = g[last_point][i];
          }
          else // Na primeira alocação considera apenas a capacidade restante
          {
            distance_by_capacity = this->capacities[j];
          }

          if (distance_by_capacity < better_distance_by_capacity)
          {
            better_distance_by_capacity = distance_by_capacity;
            better_vehicle = j;
            better_point = i;
          }
        }
      }
    }

    if (hasAvailibleVehicles)
    {
      // Atualizando a solução
      this->sequence[better_vehicle].push_back(not_visited[better_point]);
      not_visited.erase(not_visited.begin() + better_point);
    }
    else
    {
      // Adicionando rotas restantes aos terceirizados caso não tenha veiculos disponiveis
      for (int i = 0; i < not_visited.size() - 1; i++)
      {
        this->sequence[sequence.size() - 1].push_back(not_visited[i]);
        not_visited.erase(not_visited.begin() + i);
      }
    }
  }
}

Solution::Solution(/* args */)
{
  int k = Reader::instance->getMaxVehiclesQuantity();
  int n = Reader::instance->getDimension();
  this->cost = 0;
  this->vehicles = 0;
  this->sequence = vector<vector<int>>(k + 1, vector<int>(0, n + 2));
  this->capacities = vector<int>(k, 0);
}

Solution::~Solution() {}

void Solution::bestImprovementReinsertionVehicles()
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
      for (int i = 0; i < this->sequence[k1].size() - 1; i++)
      {
        int vi = this->sequence[k1][i];
        int viPrev = this->sequence[k1][i - 1];
        int viNext = this->sequence[k1][i + 1];

        int j = 0;
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
    this->cost -= bestDelta;
    int demand = Reader::instance->getDemand(this->sequence[bestK1][bestI]);
    this->capacities[bestK1] -= demand;
    this->capacities[bestK2] += demand;
    this->reinsertion(bestK1, bestI, bestK2, bestJ);
  }
}

void Solution::reinsertion(int k1, int i, int k2, int j) {
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

    for (size_t j = 0; j < this->sequence.size(); j++)
    {
      cout << " " << this->sequence[i][j] << " ";
    }
    cout << endl;
  }
}