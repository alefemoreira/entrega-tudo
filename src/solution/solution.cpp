#include "solution.h"
#include "../reader/reader.h"
#include <iostream>
#include <vector>

using namespace std;

void Solution::build() {
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
  for (int i = 1; i <= n; i++) {
    candidates.push_back(i);
  }

  for (int i = 0; i < K; i++) {
    this->sequence[i].push_back(0);
  }

  while (!candidates.empty()) {
    int betterK = 0;
    int demand = 0;
    int betterVertice = 0;
    int betterI = 0;
    double betterCost = INFINITE;

    for (int i = 0; i < candidates.size(); i++) {
      int vi = candidates[i];
      double viDemand = Reader::instance->getDemand(vi);

      for (int k = 0; k < K; k++) // veiculos
      {
        double cost = this->getCost(vi, k, viDemand);

        if (cost < betterCost) {
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

    this->cost += betterCost;

    if (betterK < maxVehicles) {
      this->deliveries += 1;

      if (this->capacities[betterK] == 0) {
        this->vehicles++;
      }

      this->capacities[betterK] += demand;
    }

    if (this->deliveries >= l && K <= maxVehicles) {
      K++;
    }
  }

  for (int i = 0; i < maxVehicles; i++) {
    int back = this->sequence[i].back();
    this->cost += Reader::instance->getDistance(back, 0);
    this->sequence[i].push_back(0);
  }
}

Solution::Solution(/* args */) {
  int k = Reader::instance->getMaxVehiclesQuantity();
  int n = Reader::instance->getDimension();
  this->cost = 0;
  this->vehicles = 0;
  this->deliveries = 0;
  this->sequence = vector<vector<int>>(k + 1, vector<int>());
  this->capacities = vector<int>(k, 0);
}

Solution::~Solution() {}

void Solution::vnd() {
  // bool improved = false;

  // do {
  //   improved = this->bestImprovementReinsertionVehicles();
  // } while (improved);
  bool improved = false;
  vector<int> counters({0, 0, 0, 0, 0});
  vector<int> NL({5, 4, 3});

  while (!NL.empty()) {
    int n = NL.back();
    NL.pop_back();

    do {
      switch (n) {
      case 1:
        break;
      case 2:
        break;
      case 3:
        improved = this->bestImprovementReinsertionVehicles();
        break;
      case 4:
        improved = this->bestImprovementOutsourcing();
        break;
      case 5:
        improved = this->bestImprovementUndoOutsourcing();
        break;
      }
    } while (improved);
  }
}

bool Solution::swapVehicles() {
  int bestI, bestJ, bestK1, bestK2;
  int bestDelta = 0;
  int vehicles = Reader::instance->getMaxVehiclesQuantity();
  double r = Reader::instance->getCarUseCost();
  int Q = Reader::instance->getCarCapacity();
  // int dimension = this->

  for (int k1 = 0; k1 < vehicles; k1++) {
    for (int k2 = k1; k2 < vehicles; k2++) {
      for (int i = 1; i < this->sequence[k1].size() - 1; i++) {
        int vi = this->sequence[k1][i];
        int viPrev = this->sequence[k1][i - 1];
        int viNext = this->sequence[k1][i + 1];

        int j = 1;
        if (k1 == k2)
          j = i + 1;

        for (; j < this->sequence[k2].size() - 1; j++) {
          int vj = this->sequence[k2][j];
          int vjNext = this->sequence[k2][j + 1];

          if (Reader::instance->getDemand(vi) + this->capacities[k2] > Q)
            continue;

          double delta = Reader::instance->getDistance(vi, vjNext) +
                         Reader::instance->getDistance(vj, vi) +
                         Reader::instance->getDistance(viPrev, viNext) -
                         Reader::instance->getDistance(viPrev, vi) -
                         Reader::instance->getDistance(vi, viNext) -
                         Reader::instance->getDistance(vj, vjNext);

          if (this->sequence[k2].size() == 2)
            delta += r;

          if (this->sequence[k1].size() == 3)
            delta -= r;

          if (delta < bestDelta) {
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
  if (bestDelta < 0) {
    this->cost -= bestDelta;
    int demand = Reader::instance->getDemand(this->sequence[bestK1][bestI]);
    this->capacities[bestK1] -= demand;
    this->capacities[bestK2] += demand;
    this->reinsertion(bestK1, bestI, bestK2, bestJ);
    return true;
  }
  return false;
}

bool Solution::bestImprovementReinsertionVehicles() {
  int bestI, bestK, bestJ, bestL, demand, deltaVehicles;
  double bestDelta = 0;

  double r = Reader::instance->getCarUseCost();
  int Q = Reader::instance->getCarCapacity();
  int o = Reader::instance->getMaxVehiclesQuantity();

  for (int k = 0; k < this->sequence.size() - 1; k++) {
    // remove do veículo k (...)
    for (int i = 1; i < this->sequence[k].size() - 1; i++) {
      int viPrev = this->sequence[k][i - 1];
      int vi = this->sequence[k][i];
      int viNext = this->sequence[k][i + 1];

      int viDemand = Reader::instance->getDemand(vi);
      double costViPrevVi = Reader::instance->getDistance(viPrev, vi);
      double costViViNext = Reader::instance->getDistance(vi, viNext);
      double costViPrevViNext = Reader::instance->getDistance(viPrev, viNext);

      // (...) e insere no veiculo l
      for (int l = 0; l < this->sequence.size() - 1; l++) {
        if (k == l || viDemand + this->capacities[l] > Q)
          continue;
        for (int j = 1; j < this->sequence[l].size() - 1; j++) {
          int vj = this->sequence[l][j];
          int vjNext = this->sequence[l][j + 1];

          double useCost = 0;
          int vehicles = 0;

          // caso em que é removido uma entrega de um carro que só tem essa
          // única entrega
          if (this->capacities[k] - viDemand == 0) {
            vehicles--;
            useCost -= r;
          }

          // caso em que é adicionado uma entrega a um carro que não é usado
          if (this->capacities[l] == 0) {
            vehicles++;
            useCost += r;
          }

          double delta = Reader::instance->getDistance(vj, vi) +
                         Reader::instance->getDistance(vi, vjNext) +
                         costViPrevViNext - costViPrevVi - costViViNext -
                         Reader::instance->getDistance(vj, vjNext) + useCost;

          if (delta < bestDelta) {
            bestK = k;
            bestL = l;
            bestI = i;
            bestJ = j;
            bestDelta = delta;
            deltaVehicles = vehicles;
          }
        }
      }
    }
  }
  if (bestDelta < 0) {
    this->cost += bestDelta;
    this->vehicles += deltaVehicles;
    this->capacities[bestK] -= demand;
    this->capacities[bestL] += demand;

    this->reinsertion(bestK, bestI, bestL, bestJ);
    return true;
  }
  return false;
}

/*
O()?
remover demand do carro k
*/
bool Solution::bestImprovementOutsourcing() {
  if (this->deliveries == Reader::instance->getMinimumDelivery())
    return false;

  int bestI, bestK, demand;
  double bestDelta = 0;

  double r = Reader::instance->getCarUseCost();

  for (int k = 0; k < this->sequence.size() - 1; k++) {
    for (int i = 1; i < this->sequence[k].size() - 1; i++) {
      int viPrev = this->sequence[k][i - 1];
      int vi = this->sequence[k][i];
      int viNext = this->sequence[k][i + 1];

      double delta = Reader::instance->getDistance(viPrev, viNext) +
                     Reader::instance->getOutsourcing(vi) -
                     Reader::instance->getDistance(viPrev, vi) -
                     Reader::instance->getDistance(vi, viNext);

      if (this->sequence[k].size() == 3) {
        delta -= r;
      }

      if (delta < bestDelta) {
        bestI = i;
        bestK = k;
        bestDelta = delta;
      }
    }
  }

  if (bestDelta < 0) {
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
bool Solution::bestImprovementUndoOutsourcing() {
  if (this->deliveries == Reader::instance->getMinimumDelivery())
    return false;

  int bestI, bestK, bestJ, demand;
  double bestDelta = 0;

  double r = Reader::instance->getCarUseCost();
  int Q = Reader::instance->getCarCapacity();
  int o = Reader::instance->getMaxVehiclesQuantity();

  for (int i = 0; i < this->sequence[o].size(); i++) {
    int vi = this->sequence[o][i];
    int viDemand = Reader::instance->getDemand(vi);

    double viOutsourcingCost = Reader::instance->getOutsourcing(vi);

    for (int k = 0; k < this->sequence.size() - 1; k++) {
      if (this->capacities[k] + viDemand > Q)
        continue;

      for (int j = 1; j < this->sequence[k].size() - 1; j++) {
        int vj = this->sequence[k][j];
        int vjNext = this->sequence[k][j + 1];

        double delta = Reader::instance->getDistance(vj, vi) +
                       Reader::instance->getDistance(vi, vjNext) -
                       viOutsourcingCost -
                       Reader::instance->getDistance(vj, vjNext);

        if (this->sequence[k].size() == 2) {
          // como i será inserido no veiculo k, é necessário verificar se
          // o veiculo está ou não em uso, pois se não estiver o custo por
          // usar o carro precisa ser somado ao delta
          delta += r;
        }

        if (delta < bestDelta) {
          bestJ = j;
          bestI = i;
          bestK = k;
          demand = viDemand;
          bestDelta = delta;
        }
      }
    }
  }

  if (bestDelta < 0) {
    this->cost += bestDelta;
    this->deliveries++;
    this->capacities[bestK] += demand;
    this->peformsRemovalOutsourcing(bestI, bestK, bestJ);
    return true;
  }

  return false;
}

void Solution::peformsReinsertionOutsourcing(int k, int i) {
  auto begin = this->sequence[k].begin();
  int vi = this->sequence[k][i];

  int o = Reader::instance->getMaxVehiclesQuantity();
  this->sequence[k].erase(begin + i);
  this->sequence[o].push_back(vi);

  this->capacities[k] -= Reader::instance->getDemand(vi);
}

void Solution::peformsRemovalOutsourcing(int i, int k, int j) {
  int o = Reader::instance->getMaxVehiclesQuantity();
  int vi = this->sequence[o][i];

  auto outsourcingBegin = this->sequence[o].begin();
  auto kBegin = this->sequence[k].begin();

  this->sequence[o].erase(outsourcingBegin + i);
  this->sequence[k].insert(kBegin + j, vi);
}

void Solution::reinsertion(int k, int i, int l, int j) {
  int v = this->sequence[k][i];
  auto kBegin = this->sequence[k].begin();
  auto lBegin = this->sequence[l].begin();

  this->sequence[k].erase(kBegin + i);
  this->sequence[l].insert(lBegin + j + 1, v);
}

void Solution::swap(int k1, int i, int k2, int j) {
  int n = this->sequence[k1][i];
  this->sequence[k1][i] = this->sequence[k2][j];
  this->sequence[k2][j] = n;
}

void Solution::setCost(double cost) { this->cost = cost; }

void Solution::setVehicles(int vehicles) { this->vehicles = vehicles; }

void Solution::setSequence(vector<vector<int>> *sequence) {
  this->sequence = *sequence;
}

double Solution::getCost(int vi, int j, double d) {
  int maxVehicles = Reader::instance->getMaxVehiclesQuantity();

  if (j == maxVehicles) {
    double cost = Reader::instance->getOutsourcing(vi);
    return cost;
  } else {
    // int d = Reader::instance->getDemand(vi);
    int last = this->sequence[j].back();
    int maxDemand = Reader::instance->getCarCapacity();

    double r = Reader::instance->getCarUseCost();
    double cost = this->capacities[j] == 0 ? r : 0;
    cost += Reader::instance->getDistance(last, vi);

    if (this->capacities[j] + d >= maxDemand) {
      return INFINITE;
    }

    return cost;
  }
}

void Solution::resume() {
  cout << "Custo: " << this->cost << endl;
  cout << "Número de veiculos: " << this->vehicles << endl;
  for (size_t i = 0; i < this->sequence.size(); i++) {
    if (i != sequence.size() - 1) {
      cout << "Rota do veiculo " << i + 1 << ":" << endl;
    } else {
      cout << "Pontos de entrega tercerizados: " << endl;
    }

    for (size_t j = 0; j < this->sequence[i].size(); j++) {
      cout << " " << this->sequence[i][j] << " ";
    }
    cout << endl;
  }
}