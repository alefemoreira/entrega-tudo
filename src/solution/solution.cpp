#include "solution.h"
#include "../reader/reader.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <sstream>
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
    // cout << "---" << better_cost << " " << better_point << endl;

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

void Solution::localSearch() {
  bool improved = false;

  int n = 1;
  int r = 6;

  while (n <= r) {
    switch (n) {
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
      improved = this->bestImprovementOrOpt(1);
      break;
    case 5:
      improved = this->bestImprovementOrOpt(2);
      break;
    case 6:
      improved = this->bestImprovementOrOpt(3);
      break;
    }
    if (improved) {
      n = 1;
    } else {
      n++;
    }
  };
}

bool Solution::bestImprovementSwap() {
  // melhor I, J e linha
  int bestI, bestJ, bestK;

  // número máximo de veículos
  int K = this->sequence.size();
  double bestDelta = 0;

  // l = rota
  for (int k = 0; k < K - 1; k++) {
    for (int i = 1; i < this->sequence[k].size() - 2; i++) {
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

      if (delta < bestDelta) {
        bestDelta = delta;
        bestI = i;
        bestJ = j;
        bestK = k;
      }

      for (j = i + 2; j < this->sequence[k].size() - 1; j++) {
        vj = this->sequence[k][j];
        vjNext = this->sequence[k][j + 1];
        vjPrev = this->sequence[k][j - 1];

        delta = Reader::instance->getDistance(viPrev, vj) +
                Reader::instance->getDistance(vj, viNext) +
                Reader::instance->getDistance(vjPrev, vi) +
                Reader::instance->getDistance(vi, vjNext) - costViPrevVi -
                costViViNext - Reader::instance->getDistance(vjPrev, vj) -
                Reader::instance->getDistance(vj, vjNext);

        if (delta < bestDelta) {
          bestDelta = delta;
          bestI = i;
          bestJ = j;
          bestK = k;
        }
      }
    }
  }

  if (bestDelta < 0) {
    this->cost += bestDelta;
    std::swap(this->sequence[bestK][bestI], this->sequence[bestK][bestJ]);

    return true;
  }

  return false;
}

bool Solution::bestImprovementTwoOpt() {
  // melhor I, J e linha
  int bestI, bestJ, bestK;

  // número máximo de veículos
  int K = this->sequence.size();
  double bestDelta = 0;

  // l = rota
  for (int k = 0; k < K - 1; k++) {
    for (int i = 1; i < this->sequence[k].size() - 2; i++) {
      int viPrev = this->sequence[k][i - 1];
      int vi = this->sequence[k][i];

      double costViPrevVi = Reader::instance->getDistance(viPrev, vi);

      for (int j = i + 2; j < this->sequence[k].size() - 1; j++) {
        int vj = this->sequence[k][j];
        int vjNext = this->sequence[k][j + 1];

        double delta = Reader::instance->getDistance(vi, vjNext) +
                       Reader::instance->getDistance(vj, viPrev) -
                       Reader::instance->getDistance(vj, vjNext) - costViPrevVi;

        if (delta < bestDelta) {
          bestDelta = delta;
          bestI = i;
          bestJ = j;
          bestK = k;
        }
      }
    }
  }

  if (bestDelta < 0) {
    this->cost += bestDelta;
    reverse(this->sequence[bestK].begin() + bestI,
            this->sequence[bestK].begin() + bestJ + 1);
    // std::swap(this->sequence[bestK][bestI], this->sequence[bestK][bestJ]);

    return true;
  }

  return false;
}

bool Solution::bestImprovementSwapVehicles() {
  int bestI, bestJ, bestK, bestL, deltaDemand;
  int bestDelta = 0;
  int K = Reader::instance->getMaxVehiclesQuantity();
  double r = Reader::instance->getCarUseCost();
  int Q = Reader::instance->getCarCapacity();
  // int dimension = this->

  for (int k = 0; k < this->sequence.size() - 1; k++) {
    for (int i = 1; i < this->sequence[k].size() - 1; i++) {
      int vi = this->sequence[k][i];
      int viPrev = this->sequence[k][i - 1];
      int viNext = this->sequence[k][i + 1];
      int viDemand = Reader::instance->getDemand(vi);

      double costViPrevVi = Reader::instance->getDistance(viPrev, vi);
      double costViViNext = Reader::instance->getDistance(vi, viNext);

      int currentKCapacity = this->capacities[k] - viDemand;

      for (int l = k + 1; l < this->sequence.size() - 1; l++) {
        for (int j = 1; j < this->sequence[l].size() - 1; j++) {
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

          if (delta < bestDelta) {
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

  if (bestDelta < 0) {

    this->cost += bestDelta;
    this->capacities[bestK] += deltaDemand;
    this->capacities[bestL] -= deltaDemand;

    std::swap(this->sequence[bestK][bestI], this->sequence[bestL][bestJ]);
    return true;
  }
  return false;
}

bool Solution::feasible() {
  int deliveries = 0;
  int L = Reader::instance->getMinimumDelivery();
  int Q = Reader::instance->getCarCapacity();

  double originalCost = this->cost;
  double calculatedCost = this->calculateCost();

  if (originalCost != calculatedCost) {
    cout << " Custo errado " << originalCost << "!=" << calculatedCost << " ";
    return false;
  }

  for (int k = 0; k < this->sequence.size() - 1; k++) {
    deliveries += this->sequence[k].size() - 2;
    int demand = 0;

    for (int i = 1; i < this->sequence[k].size() - 1; i++) {
      demand += Reader::instance->getDemand(this->sequence[k][i]);
    }
    if (demand != this->capacities[k]) {
      cout << " " << this->capacities[k] << "!=" << demand
           << " Demand incoerente: " << k << "-";
      this->capacities[k] = demand;
      return false;
    }
    if (demand > Q) {
      cout << " " << demand << " Demanda estourou de: " << k << " ";
      return false;
    }
  }

  if (deliveries < L) {
    cout << " Entregas insuficientes ";
    return false;
  }

  return true;
}

/*
 size = {2, 3}
*/
bool Solution::bestImprovementOrOpt(int size) {
  int bestI, bestK, bestJ, bestL, demand, deltaVehicles, mode;
  double bestDelta = 0;

  double r = Reader::instance->getCarUseCost();
  int Q = Reader::instance->getCarCapacity();
  int O = this->sequence.size() - 1;
  int o = Reader::instance->getMaxVehiclesQuantity();
  int L = Reader::instance->getMinimumDelivery();

  for (int k = 0; k < this->sequence.size() - 1; k++) {
    // remove do veículo k (...)
    for (int i = 1; i < (int)this->sequence[k].size() - size; i++) {
      int viPrev = this->sequence[k][i - 1];
      int vi = this->sequence[k][i];

      int vi2 = this->sequence[k][i + size - 1];
      int vi2Next = this->sequence[k][i + size];

      int segmentDemand = 0;

      for (int m = 0; m < size; m++) {
        segmentDemand += Reader::instance->getDemand(this->sequence[k][i + m]);
      }

      double costViPrevVi = Reader::instance->getDistance(viPrev, vi);
      double costViPrevVi2Next = Reader::instance->getDistance(viPrev, vi2Next);
      double costVi2Vi2Next = Reader::instance->getDistance(vi2, vi2Next);

      // (...) e insere no veiculo l
      for (int l = 0; l < this->sequence.size() - 1; l++) {
        if (k == l || segmentDemand + this->capacities[l] > Q)
          continue;

        for (int j = 1; j < this->sequence[l].size() - 1; j++) {
          int vj = this->sequence[l][j];
          int vjNext = this->sequence[l][j + 1];

          double useCost = 0;
          int vehicles = 0;

          // caso em que é removido uma entrega de um carro que só tem essa
          // única entrega
          if (this->capacities[k] - segmentDemand == 0) {
            vehicles--;
            useCost -= r;
          }

          // caso em que é adicionado uma entrega a um carro que não é usado
          if (this->capacities[l] == 0) {
            vehicles++;
            useCost += r;
          }

          double delta =
              costViPrevVi2Next + Reader::instance->getDistance(vj, vi) +
              Reader::instance->getDistance(vi2, vjNext) - costViPrevVi -
              Reader::instance->getDistance(vj, vjNext) - costVi2Vi2Next;

          if (delta < bestDelta) {
            bestK = k;
            bestL = l;
            bestI = i;
            bestJ = j;
            demand = segmentDemand;
            bestDelta = delta;
            deltaVehicles = vehicles;
            mode = 0;
          }
        }
      }

      if (this->deliveries - size < L) // evita a inviabilidade
        continue;

      double segmentOutsourcing = 0, segmentCost = 0;

      for (int m = 0; m < size; m++) {
        segmentOutsourcing +=
            Reader::instance->getOutsourcing(this->sequence[k][i + m]);
      }

      for (int m = 0; m < size - 1; m++) {
        segmentCost += Reader::instance->getDistance(
            this->sequence[k][i + m], this->sequence[k][i + m + 1]);
      }

      // caso de terceirizar o segmento
      double delta_ = costViPrevVi2Next + segmentOutsourcing - costViPrevVi -
                      costVi2Vi2Next - segmentCost;

      if (this->sequence[k].size() == 2 + size) {
        delta_ -= r;
      }

      if (delta_ < bestDelta) {
        bestI = i;
        bestK = k;
        demand = segmentDemand;
        bestDelta = delta_;
        mode = 1;
      }
    }
  }

  // caso de remover da terceirização
  for (int i = 0; i < this->sequence[O].size() - size + 1; i++) {
    int vi = this->sequence[O][i];
    int vi2 = this->sequence[O][i + size - 1];
    // int segmentDemand = Reader::instance->getDemand(vi);
    int segmentDemand = 0;
    double segmentOutsourcing = 0;
    double segmentCost = 0;

    for (int m = 0; m < size; m++) {
      segmentDemand += Reader::instance->getDemand(this->sequence[O][i + m]);
      segmentOutsourcing +=
          Reader::instance->getOutsourcing(this->sequence[O][i + m]);
    }
    for (int m = 0; m < size - 1; m++) {
      segmentCost += Reader::instance->getDistance(
          this->sequence[O][i + m], this->sequence[O][i + m + 1]);
    }

    for (int k = 0; k < O; k++) {
      if (this->capacities[k] + segmentDemand > Q)
        continue;

      for (int j = 1; j < this->sequence[k].size() - 1; j++) {
        int vj = this->sequence[k][j];
        int vjNext = this->sequence[k][j + 1];

        double delta = Reader::instance->getDistance(vj, vi) + segmentCost +
                       Reader::instance->getDistance(vi2, vjNext) -
                       segmentOutsourcing -
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
          demand = segmentDemand;
          bestDelta = delta;
          mode = 2;
        }
      }
    }
  }

  if (bestDelta < 0) {
    // mode pertence ao conjunto {0, 1, 2}
    if (mode == 0) {
      this->cost += bestDelta;
      this->vehicles += deltaVehicles;
      this->capacities[bestK] -= demand;
      this->capacities[bestL] += demand;

      // printf("%d %d %d %d\n", bestK, bestI, bestL, bestJ);
      this->reinsertion(bestK, bestI, bestL, bestJ, size);
      return true;
    } else if (mode == 1) {
      this->cost += bestDelta;
      this->capacities[bestK] -= demand;
      this->deliveries -= size;
      this->peformsReinsertionOutsourcing(bestK, bestI, size);
      return true;
    } else {
      this->cost += bestDelta;
      this->deliveries += size;
      this->capacities[bestK] += demand;
      this->peformsRemovalOutsourcing(bestI, bestK, bestJ, size);
      return true;
    }
  }
  return false;
}

void Solution::peformsReinsertionOutsourcing(int k, int i, int size) {
  int o = Reader::instance->getMaxVehiclesQuantity();

  for (int m = 0; m < size; m++) {
    int v = this->sequence[k][i];
    this->sequence[k].erase(this->sequence[k].begin() + i);
    this->sequence[o].push_back(v);
  }
}

void Solution::peformsRemovalOutsourcing(int i, int k, int j, int size) {
  int o = Reader::instance->getMaxVehiclesQuantity();

  for (int m = 0; m < size; m++) {
    int v = this->sequence[o][i];
    this->sequence[o].erase(this->sequence[o].begin() + i);
    this->sequence[k].insert(this->sequence[k].begin() + j + 1 + m, v);
  }
}

void Solution::reinsertion(int k, int i, int l, int j, int size) {
  vector<int> v(size, 0);

  for (int m = 0; m < size; m++) {
    v[m] = *(this->sequence[k].begin() + i + m);
  }

  this->sequence[k].erase(this->sequence[k].begin() + i,
                          this->sequence[k].begin() + i + size);

  for (int m = 0; m < size; m++) {
    int value = v[m];

    this->sequence[l].insert(this->sequence[l].begin() + j + 1 + m, value);
  }
}

void Solution::setCost(double cost) { this->cost = cost; }

void Solution::setVehicles(int vehicles) { this->vehicles = vehicles; }

void Solution::setSequence(vector<vector<int>> *sequence) {
  this->sequence = *sequence;
}

double Solution::calculateCost() {
  this->cost = 0;
  this->deliveries = 0;
  this->vehicles = 0;
  double r = Reader::instance->getCarUseCost();
  int K = this->sequence.size();

  for (int k = 0; k < K - 1; k++) {
    this->capacities[k] = 0;

    if (this->sequence[k].size() > 2) {
      this->cost += r;
      this->vehicles++;
      this->deliveries += this->sequence[k].size() - 2;
    }

    for (int i = 0, j = 1; j < this->sequence[k].size(); i++, j++) {
      this->cost += Reader::instance->getDistance(this->sequence[k][i],
                                                  this->sequence[k][j]);
    }

    for (int i = 1; i < this->sequence[k].size() - 1; i++) {
      this->capacities[k] += Reader::instance->getDemand(this->sequence[k][i]);
    }
  }

  for (int i = 0; i < this->sequence[K - 1].size(); i++)
    this->cost += Reader::instance->getOutsourcing(this->sequence[K - 1][i]);

  return this->cost;
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

Solution::Solution(Solution *s) {
  sequence = s->getSequence();
  this->cost = s->Cost();
  this->vehicles = s->getVehicles();
  this->deliveries = s->getDeliveries();
  this->capacities = s->getCapacities();
}

Solution *Solution::disturbance(Solution *s) {
  Solution *p = new Solution(s);
  p->disturbance();
  return p;
}

void Solution::disturbance() {
  // Carro 1
  int k = rand() % (this->sequence.size() - 1);

  while (this->sequence[k].size() - 2 <= 0) {
    k = rand() % (this->sequence.size() - 1);
  }

  // Bloco do carro 1
  int maxSize = max(1, int(floor(this->sequence[k].size() / 10)));
  int size1 = 1 + rand() % maxSize;
  int i = rand() % (this->sequence[k].size() - 2 - size1) + 1;

  // Carro 2
  int l = rand() % (this->sequence.size() - 1);
  while (l == k) {
    l = rand() % (this->sequence.size() - 1);
  }

  // Blocos
  int Q = Reader::instance->getCarCapacity();
  int r = Reader::instance->getCarUseCost();
  vector<int> blockK;
  int blockCapacity = 0;

  for (int v = i; v < i + size1; v++) {
    blockK.push_back(this->sequence[k][v]);
    blockCapacity += Reader::instance->getDemand(this->sequence[k][v]);
  }

  // Validando as capacidades
  if (this->capacities[l] + blockCapacity <= Q) {
    double delta = 0;

    int viPrev = this->sequence[k][i - 1];
    int vi = this->sequence[k][i];

    int vi2 = this->sequence[k][i + size1 - 1];
    int vi2Next = this->sequence[k][i + size1];

    int j = rand() % (this->sequence[l].size() - 1);

    if (this->sequence[l].size() == 2) {
      j = 0;
      delta += r;
    }

    int vj = this->sequence[l][j];
    int vjNext = this->sequence[l][j + 1];

    // Trocando os blocos
    this->sequence[k].erase(this->sequence[k].begin() + i,
                            this->sequence[k].begin() + i + size1);

    this->sequence[l].insert(this->sequence[l].begin() + j + 1, blockK.begin(),
                             blockK.end());

    this->capacities[l] += blockCapacity;
    this->capacities[k] -= blockCapacity;

    delta += Reader::instance->getDistance(viPrev, vi2Next) +
             Reader::instance->getDistance(vj, vi) +
             Reader::instance->getDistance(vi2, vjNext) -
             Reader::instance->getDistance(viPrev, vi) -
             Reader::instance->getDistance(vj, vjNext) -
             Reader::instance->getDistance(vi2, vi2Next);

    if (sequence[k].size() == 2) {
      delta -= r;
    }

    this->cost += delta;
  }
}

std::string convertTimeTToString(time_t t) {
  struct tm *timeinfo = localtime(&t);
  std::ostringstream oss;

  oss << std::setw(4) << std::setfill('0') << timeinfo->tm_year + 1900
      << std::setw(2) << std::setfill('0') << timeinfo->tm_mon + 1
      << std::setw(2) << std::setfill('0') << timeinfo->tm_mday << std::setw(2)
      << std::setfill('0') << timeinfo->tm_hour << std::setw(2)
      << std::setfill('0') << timeinfo->tm_min;

  return oss.str();
}

void Solution::writeSolution() {
  std::time_t t = std::time(0);

  string filename = "out/" + Reader::instance->getInstanceName() +
                    "_solution_" + convertTimeTToString(t) + ".txt";

  ofstream file;
  file.open(filename);

  if (file.is_open()) {
    double costRoute = 0; // custo de roteamento
    double outsourcingCost = 0;
    double costVehicles = Reader::instance->getCarUseCost() * this->vehicles;

    string routes = "";
    string outsourcing = "";
    int K = this->sequence.size();

    for (int k = 0; k < this->vehicles; k++) {
      for (int i = 0, j = 1; j < this->sequence[k].size(); i++, j++) {
        costRoute += Reader::instance->getDistance(this->sequence[k][i],
                                                   this->sequence[k][j]);
        if (this->sequence[k][i] != 0) {
          routes += to_string(this->sequence[k][i]) + " ";
        }
      }

      routes += "\n";
    }

    for (int i = 0; i < this->sequence[K - 1].size(); i++) {
      outsourcingCost +=
          Reader::instance->getOutsourcing(this->sequence[K - 1][i]);
      outsourcing += to_string(this->sequence[K - 1][i]) + " ";
    }

    file << this->cost << endl;
    file << costRoute << endl;
    file << costVehicles << endl;
    file << outsourcingCost << endl;
    file << endl;
    file << outsourcing << endl;
    file << endl;
    file << this->vehicles << endl;
    file << routes << endl;

    file.close();
  }
}