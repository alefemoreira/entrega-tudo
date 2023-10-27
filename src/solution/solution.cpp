#include "solution.h"
#include "../reader/reader.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

void Solution::build()
{
  this->cost = 0;
  int n = Reader::instance->getDimension();
  int K = Reader::instance->getMaxVehiclesQuantity();
  int l = Reader::instance->getMinimumDelivery();

  int maxVehicles = K;
  int maxVehicleCapacity = Reader::instance->getCarCapacity();
  this->deliveries = 0;
  this->vehicles = 0;

  // Pontos não visitados
  vector<int> candidates;
  for (int i = 1; i <= n; i++)
  {
    candidates.push_back(i);
  }

  for (int i = 0; i < K; i++)
  {
    this->sequence[i].push_back(0);
  }

  while (!candidates.empty())
  {
    int betterK = 0;
    int demand = 0;
    int betterVertice = 0;
    int betterI = 0;
    double betterCost = INFINITE;

    for (int i = 0; i < candidates.size(); i++)
    {
      int vi = candidates[i];
      double viDemand = Reader::instance->getDemand(vi);

      for (int k = 0; k < K; k++) // veiculos
      {
        double cost = this->getCost(vi, k, viDemand);

        if (cost < betterCost)
        {
          betterCost = cost;
          betterVertice = vi;
          betterI = i;
          demand = viDemand;
          betterK = k;
        }
      }
    }

    // Atualizando a solução
    this->sequence[betterK].push_back(betterVertice);

    candidates.erase(candidates.begin() + betterI);
    // cout << "---" << better_cost << " " << better_point << endl;

    this->cost += betterCost;

    if (betterK < maxVehicles)
    {
      this->deliveries += 1;

      if (this->capacities[betterK] == 0)
      {
        this->vehicles++;
      }

      this->capacities[betterK] += demand;
    }

    if (this->deliveries >= l && K <= maxVehicles)
    {
      K++;
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
  this->deliveries = 0;
  this->sequence = vector<vector<int>>(k + 1, vector<int>());
  this->capacities = vector<int>(k, 0);
}

Solution::~Solution() {}

void Solution::localSearch()
{
  std::vector<int> NL = {1, 2, 3, 4, 5};
  bool improved = false;

  // do {
  //   improved = bestImprovementSwapVehicles();
  // } while (improved);
  // return;

  int n = 1;
  int r = 6;

  while (n <= r)
  {
    switch (n)
    {
    case 1:
      improved = this->bestImprovementSwap();

      break;
    case 2:
      improved = this->bestImprovementTwoOpt();

      break;
    case 3:
      improved = this->bestImprovementSwapVehicles();

      break;
    case 4:
      improved = this->bestImprovementReinsertionVehicles();

      break;
    case 5:
      improved = this->bestImprovementOutsourcing();

      break;
    case 6:
      improved = this->bestImprovementUndoOutsourcing();
      break;
    }
    // cout << n << " ";

    // cout << n << endl;
    if (improved)
    {
      // if (!this->feasible()) {
      //   cout << n << " Errou feio, errou rude" << endl;
      //   exit(1);
      // }
      // cout << n << " ";
      n = 1;
    }
    else
    {
      n++;
    }
  };
}

bool Solution::bestImprovementSwap()
{
  // melhor I, J e linha
  int bestI, bestJ, bestK;

  // número máximo de veículos
  int K = this->sequence.size();
  double bestDelta = 0;

  // l = rota
  for (int k = 0; k < K - 1; k++)
  {
    for (int i = 1; i < this->sequence[k].size() - 2; i++)
    {
      int vi = this->sequence[k][i];
      int viNext = this->sequence[k][i + 1];
      int viPrev = this->sequence[k][i - 1];

      double costViPrevVi = Reader::instance->getDistance(viPrev, vi);
      double costViViNext = Reader::instance->getDistance(vi, viNext);

      int j = i + 1;
      int vj = this->sequence[k][j];
      int vjNext = this->sequence[k][j + 1];
      int vjPrev = this->sequence[k][j - 1];

      double delta = Reader::instance->getDistance(viPrev, vj) +
                     Reader::instance->getDistance(vj, vi) +
                     Reader::instance->getDistance(vi, vjNext) - costViPrevVi -
                     Reader::instance->getDistance(vi, vj) -
                     Reader::instance->getDistance(vj, vjNext);

      if (delta < bestDelta)
      {
        bestDelta = delta;
        bestI = i;
        bestJ = j;
        bestK = k;
      }

      for (j = i + 2; j < this->sequence[k].size() - 1; j++)
      {
        vj = this->sequence[k][j];
        vjNext = this->sequence[k][j + 1];
        vjPrev = this->sequence[k][j - 1];

        delta = Reader::instance->getDistance(viPrev, vj) +
                Reader::instance->getDistance(vj, viNext) +
                Reader::instance->getDistance(vjPrev, vi) +
                Reader::instance->getDistance(vi, vjNext) - costViPrevVi -
                costViViNext - Reader::instance->getDistance(vjPrev, vj) -
                Reader::instance->getDistance(vj, vjNext);

        if (delta < bestDelta)
        {
          bestDelta = delta;
          bestI = i;
          bestJ = j;
          bestK = k;
        }
      }
    }
  }

  if (bestDelta < 0)
  {
    this->cost += bestDelta;
    std::swap(this->sequence[bestK][bestI], this->sequence[bestK][bestJ]);

    return true;
  }

  return false;
}

bool Solution::bestImprovementTwoOpt()
{
  // melhor I, J e linha
  int bestI, bestJ, bestK;

  // número máximo de veículos
  int K = this->sequence.size();
  double bestDelta = 0;

  // l = rota
  for (int k = 0; k < K - 1; k++)
  {
    for (int i = 1; i < this->sequence[k].size() - 2; i++)
    {
      int viPrev = this->sequence[k][i - 1];
      int vi = this->sequence[k][i];

      double costViPrevVi = Reader::instance->getDistance(viPrev, vi);

      for (int j = i + 2; j < this->sequence[k].size() - 1; j++)
      {
        int vj = this->sequence[k][j];
        int vjNext = this->sequence[k][j + 1];

        double delta = Reader::instance->getDistance(vi, vjNext) +
                       Reader::instance->getDistance(vj, viPrev) -
                       Reader::instance->getDistance(vj, vjNext) - costViPrevVi;

        if (delta < bestDelta)
        {
          bestDelta = delta;
          bestI = i;
          bestJ = j;
          bestK = k;
        }
      }
    }
  }

  if (bestDelta < 0)
  {
    this->cost += bestDelta;
    reverse(this->sequence[bestK].begin() + bestI,
            this->sequence[bestK].begin() + bestJ + 1);
    // std::swap(this->sequence[bestK][bestI], this->sequence[bestK][bestJ]);

    return true;
  }

  return false;
}

bool Solution::bestImprovementSwapVehicles()
{
  int bestI, bestJ, bestK, bestL, deltaDemand;
  int bestDelta = 0;
  int K = Reader::instance->getMaxVehiclesQuantity();
  double r = Reader::instance->getCarUseCost();
  int Q = Reader::instance->getCarCapacity();
  // int dimension = this->

  for (int k = 0; k < this->sequence.size() - 1; k++)
  {
    for (int i = 1; i < this->sequence[k].size() - 1; i++)
    {
      int vi = this->sequence[k][i];
      int viPrev = this->sequence[k][i - 1];
      int viNext = this->sequence[k][i + 1];
      int viDemand = Reader::instance->getDemand(vi);

      double costViPrevVi = Reader::instance->getDistance(viPrev, vi);
      double costViViNext = Reader::instance->getDistance(vi, viNext);

      int currentKCapacity = this->capacities[k] - viDemand;

      for (int l = k + 1; l < this->sequence.size() - 1; l++)
      {
        for (int j = 1; j < this->sequence[l].size() - 1; j++)
        {
          int vj = this->sequence[l][j];
          int vjDemand = Reader::instance->getDemand(vj);

          int currentLCapacity = this->capacities[l] - vjDemand;
          if (currentLCapacity + viDemand > Q ||
              currentKCapacity + vjDemand > Q)
            continue;

          int vjPrev = this->sequence[l][j - 1];
          int vjNext = this->sequence[l][j + 1];

          double costVjPrevVj = Reader::instance->getDistance(vjPrev, vj);
          double costVjVjNext = Reader::instance->getDistance(vj, vjNext);

          double delta = Reader::instance->getDistance(viPrev, vj) +
                         Reader::instance->getDistance(vj, viNext) +
                         Reader::instance->getDistance(vjPrev, vi) +
                         Reader::instance->getDistance(vi, vjNext) -
                         costViPrevVi - costViViNext - costVjPrevVj -
                         costVjVjNext;

          if (delta < bestDelta)
          {
            bestDelta = delta;
            bestI = i;
            bestJ = j;
            bestK = k;
            bestL = l;
            deltaDemand = vjDemand - viDemand;
            // iDemand = viDemand;
            // jDemand = vjDemand;
          }
        }
      }
    }
  }

  if (bestDelta < 0)
  {

    this->cost += bestDelta;
    this->capacities[bestK] += deltaDemand;
    this->capacities[bestL] -= deltaDemand;

    std::swap(this->sequence[bestK][bestI], this->sequence[bestL][bestJ]);
    return true;
  }
  return false;
}

bool Solution::bestImprovementReinsertionVehicles()
{
  int bestI, bestK, bestJ, bestL, demand, deltaVehicles;
  double bestDelta = 0;

  double r = Reader::instance->getCarUseCost();
  int Q = Reader::instance->getCarCapacity();
  int o = Reader::instance->getMaxVehiclesQuantity();

  for (int k = 0; k < this->sequence.size() - 1; k++)
  {
    // remove do veículo k (...)
    for (int i = 1; i < this->sequence[k].size() - 1; i++)
    {
      int viPrev = this->sequence[k][i - 1];
      int vi = this->sequence[k][i];
      int viNext = this->sequence[k][i + 1];

      int viDemand = Reader::instance->getDemand(vi);
      double costViPrevVi = Reader::instance->getDistance(viPrev, vi);
      double costViViNext = Reader::instance->getDistance(vi, viNext);
      double costViPrevViNext = Reader::instance->getDistance(viPrev, viNext);

      // (...) e insere no veiculo l
      for (int l = 0; l < this->sequence.size() - 1; l++)
      {
        if (k == l || viDemand + this->capacities[l] > Q)
          continue;
        for (int j = 1; j < this->sequence[l].size() - 1; j++)
        {
          int vj = this->sequence[l][j];
          int vjNext = this->sequence[l][j + 1];

          double useCost = 0;
          int vehicles = 0;

          // caso em que é removido uma entrega de um carro que só tem essa
          // única entrega
          if (this->capacities[k] - viDemand == 0)
          {
            vehicles--;
            useCost -= r;
          }

          // caso em que é adicionado uma entrega a um carro que não é usado
          if (this->capacities[l] == 0)
          {
            vehicles++;
            useCost += r;
          }

          double delta = Reader::instance->getDistance(vj, vi) +
                         Reader::instance->getDistance(vi, vjNext) +
                         costViPrevViNext - costViPrevVi - costViViNext -
                         Reader::instance->getDistance(vj, vjNext) + useCost;

          if (delta < bestDelta)
          {
            bestK = k;
            bestL = l;
            bestI = i;
            bestJ = j;
            demand = viDemand;
            bestDelta = delta;
            deltaVehicles = vehicles;
          }
        }
      }
    }
  }
  if (bestDelta < 0)
  {
    this->cost += bestDelta;
    this->vehicles += deltaVehicles;
    this->capacities[bestK] -= demand;
    this->capacities[bestL] += demand;

    this->reinsertion(bestK, bestI, bestL, bestJ);
    return true;
  }
  return false;
}

bool Solution::feasible()
{
  int deliveries = 0;
  int L = Reader::instance->getMinimumDelivery();
  int Q = Reader::instance->getCarCapacity();

  double originalCost = this->cost;
  double calculatedCost = this->calculateCost();

  if (originalCost != calculatedCost)
  {
    cout << " Custo errado ";
    return false;
  }

  for (int k = 0; k < this->sequence.size() - 1; k++)
  {
    deliveries += this->sequence[k].size() - 2;
    int demand = 0;

    for (int i = 1; i < this->sequence[k].size() - 1; i++)
    {
      demand += Reader::instance->getDemand(this->sequence[k][i]);
    }
    if (demand != this->capacities[k])
    {
      cout << " " << this->capacities[k] << "!=" << demand
           << " Demand incoerente: " << k << "-";
      this->capacities[k] = demand;
      return false;
    }
    if (demand > Q)
    {
      cout << " " << demand << " Demanda estourou de: " << k << " ";
      return false;
    }
  }

  if (deliveries < L)
  {
    cout << " Entregas insuficientes ";
    return false;
  }

  return true;
}

/*
O()?
remover demand do carro k
*/
bool Solution::bestImprovementOutsourcing()
{
  if (this->deliveries == Reader::instance->getMinimumDelivery())
    return false;

  int bestI, bestK, demand;
  double bestDelta = 0;

  double r = Reader::instance->getCarUseCost();

  for (int k = 0; k < this->sequence.size() - 1; k++)
  {
    for (int i = 1; i < this->sequence[k].size() - 1; i++)
    {
      int viPrev = this->sequence[k][i - 1];
      int vi = this->sequence[k][i];
      int viNext = this->sequence[k][i + 1];

      double delta = Reader::instance->getDistance(viPrev, viNext) +
                     Reader::instance->getOutsourcing(vi) -
                     Reader::instance->getDistance(viPrev, vi) -
                     Reader::instance->getDistance(vi, viNext);

      if (this->sequence[k].size() == 3)
      {
        delta -= r;
      }

      if (delta < bestDelta)
      {
        bestI = i;
        bestK = k;
        bestDelta = delta;
      }
    }
  }

  if (bestDelta < 0)
  {
    this->cost += bestDelta;
    this->deliveries--;
    this->peformsReinsertionOutsourcing(bestK, bestI);
    return true;
  }

  return false;
}

/*
O()?
*/
bool Solution::bestImprovementUndoOutsourcing()
{
  if (this->deliveries == Reader::instance->getMinimumDelivery())
    return false;

  int bestI, bestK, bestJ, demand;
  double bestDelta = 0;

  double r = Reader::instance->getCarUseCost();
  int Q = Reader::instance->getCarCapacity();
  int O = this->sequence.size() - 1;

  for (int i = 0; i < this->sequence[O].size(); i++)
  {
    int vi = this->sequence[O][i];
    int viDemand = Reader::instance->getDemand(vi);

    double viOutsourcingCost = Reader::instance->getOutsourcing(vi);

    for (int k = 0; k < O; k++)
    {
      if (this->capacities[k] + viDemand > Q)
        continue;

      for (int j = 1; j < this->sequence[k].size() - 1; j++)
      {
        int vj = this->sequence[k][j];
        int vjNext = this->sequence[k][j + 1];

        double delta = Reader::instance->getDistance(vj, vi) +
                       Reader::instance->getDistance(vi, vjNext) -
                       viOutsourcingCost -
                       Reader::instance->getDistance(vj, vjNext);

        if (this->sequence[k].size() == 2)
        {
          // como i será inserido no veiculo k, é necessário verificar se
          // o veiculo está ou não em uso, pois se não estiver o custo por
          // usar o carro precisa ser somado ao delta
          delta += r;
        }

        if (delta < bestDelta)
        {
          bestJ = j;
          bestI = i;
          bestK = k;
          demand = viDemand;
          bestDelta = delta;
        }
      }
    }
  }

  if (bestDelta < 0)
  {
    this->cost += bestDelta;
    this->deliveries++;
    this->capacities[bestK] += demand;
    this->peformsRemovalOutsourcing(bestI, bestK, bestJ);
    return true;
  }

  return false;
}

void Solution::peformsReinsertionOutsourcing(int k, int i)
{
  auto begin = this->sequence[k].begin();
  int vi = this->sequence[k][i];

  int o = Reader::instance->getMaxVehiclesQuantity();
  this->sequence[k].erase(begin + i);
  this->sequence[o].push_back(vi);

  this->capacities[k] -= Reader::instance->getDemand(vi);
}

void Solution::peformsRemovalOutsourcing(int i, int k, int j)
{
  int o = Reader::instance->getMaxVehiclesQuantity();
  int vi = this->sequence[o][i];

  auto outsourcingBegin = this->sequence[o].begin();
  auto kBegin = this->sequence[k].begin();

  this->sequence[o].erase(outsourcingBegin + i);
  this->sequence[k].insert(kBegin + j + 1, vi);
}

void Solution::reinsertion(int k, int i, int l, int j)
{
  int v = this->sequence[k][i];
  auto kBegin = this->sequence[k].begin();
  auto lBegin = this->sequence[l].begin();

  this->sequence[k].erase(kBegin + i);
  this->sequence[l].insert(lBegin + j + 1, v);
}

void Solution::swap(int k1, int i, int k2, int j)
{
  int n = this->sequence[k1][i];
  this->sequence[k1][i] = this->sequence[k2][j];
  this->sequence[k2][j] = n;
}

void Solution::setCost(double cost) { this->cost = cost; }

void Solution::setVehicles(int vehicles) { this->vehicles = vehicles; }

void Solution::setSequence(vector<vector<int>> *sequence)
{
  this->sequence = *sequence;
}

double Solution::calculateCost()
{
  this->cost = 0;
  double r = Reader::instance->getCarUseCost();
  int K = this->sequence.size();

  for (int k = 0; k < K - 1; k++)
  {

    if (this->sequence[k].size() > 2)
      this->cost += r;

    for (int i = 0, j = 1; j < this->sequence[k].size(); i++, j++)
      this->cost += Reader::instance->getDistance(this->sequence[k][i],
                                                  this->sequence[k][j]);
  }

  for (int i = 0; i < this->sequence[K - 1].size(); i++)
    this->cost += Reader::instance->getOutsourcing(this->sequence[K - 1][i]);

  return this->cost;
}

double Solution::getCost(int vi, int j, double d)
{
  int maxVehicles = Reader::instance->getMaxVehiclesQuantity();

  if (j == maxVehicles)
  {
    double cost = Reader::instance->getOutsourcing(vi);
    return cost;
  }
  else
  {
    // int d = Reader::instance->getDemand(vi);
    int last = this->sequence[j].back();
    int maxDemand = Reader::instance->getCarCapacity();

    double r = Reader::instance->getCarUseCost();
    double cost = this->capacities[j] == 0 ? r : 0;
    cost += Reader::instance->getDistance(last, vi);

    if (this->capacities[j] + d >= maxDemand)
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

Solution::Solution(Solution *s)
{
  sequence = s->getSequence();
  this->cost = s->Cost();
  this->vehicles = s->getVehicles();
  this->deliveries = s->getDeliveries();
  this->capacities = s->getCapacities();
}

Solution *Solution::disturbance(Solution *s)
{
  Solution *p = new Solution(s);
  p->disturbance();
  return p;
}

struct DisturbancePositions
{
  int k;
  int i;
  int size;
};

void Solution::disturbance()
{
  cout << "Chegou" << endl;
  // Carro 1
  int k = rand() % this->sequence.size();
  while (this->sequence[k].size() == 2)
  {
    k = rand() % this->sequence.size();
  }

  // Bloco do carro 1
  int i = rand() % (this->sequence[k].size() - 2) + 1;
  int maxSize = this->sequence[k].size() / 10;
  int size1 = rand() % (max(2, maxSize) - 2 + 1) + 2;

  // Carro 2
  int l = rand() % this->sequence.size();
  while (this->sequence[k].size() == 2 && l != k)
  {
    l = rand() % this->sequence.size();
  }

  // Bloco do carro 2
  int j = rand() % (this->sequence[l].size() - 2) + 1;
  int maxSize2 = this->sequence[l].size() / 10;
  int size2 = rand() % (max(2, maxSize2) - 2 + 1) + 2;

  // Blocos
  vector<int> block1, block2;

  for (int v = i; v < i + size1; v++)
  {
    block1.push_back(this->sequence[k][i]);
  }

  for (int v = i; v < j + size2; v++)
  {
    block2.push_back(this->sequence[l][i]);
  }

  cout << "Carro k antes da troca: ";
  for (size_t i = 0; i < this->sequence[k].size(); i++)
  {
    cout << this->sequence[k][i] << " ";
  }
  cout << endl;

  cout << "Carro l antes da troca: ";
  for (size_t i = 0; i < this->sequence[l].size(); i++)
  {
    cout << this->sequence[l][i] << " ";
  }
  cout << endl;

  // Trocando os blocos
  this->sequence[k].erase(this->sequence[k].begin() + i, this->sequence[k].begin() + i + size1);
  this->sequence[l].erase(this->sequence[l].begin() + j, this->sequence[l].begin() + j + size2);

  cout << "Carro k depois da remoção de " << i << " até " << i + size1 << "(" <<  this->sequence[k][i] << ", " << this->sequence[k][i+size1] << "): ";
  for (size_t i = 0; i < this->sequence[k].size(); i++)
  {
    cout << this->sequence[k][i] << " ";
  }
  cout << endl;

  cout << "Carro k depois da remoção de " << j << " até " << j + size2 << "(" <<  this->sequence[l][j] << ", " << this->sequence[l][j+size2] << "): ";
  for (size_t i = 0; i < this->sequence[l].size(); i++)
  {
    cout << this->sequence[l][i] << " ";
  }
  cout << endl;

  this->sequence[k].insert(this->sequence[k].end(), block1.begin(), block1.end());
  this->sequence[l].insert(this->sequence[l].end(), block2.begin(), block2.end());
}
