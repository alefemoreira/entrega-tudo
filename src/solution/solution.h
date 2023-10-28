#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <vector>

class Solution
{
private:
  double cost;    // custo da solução
  int vehicles;   // qtd de vehicles usada pela solução
  int deliveries; // qtd de entregas ñ terceirizadas da solução

  std::vector<std::vector<int>> sequence; // (k + 1) x (n + 2)
  std::vector<int>
      capacities; // quanto da capacidade de cada veículo é utilizada;
  double getCost(int i, int j, double d);

  bool bestImprovementSwap();
  bool bestImprovementTwoOpt();
  bool bestImprovementOrOpt(int size);
  bool bestImprovementSwapVehicles();

  void reinsertion(int k, int i, int l, int j, int size);
  void peformsReinsertionOutsourcing(int k, int i, int size);
  void disturbance();
  std::vector<int> copyBlock(std::vector<int> &vetor, int start, int length);

  /*
    Remove elemento outsouring[i] e insere em sequence[k][j];
    O(n)
  */
  void peformsRemovalOutsourcing(int i, int k, int j, int size);

public:
  Solution(/* args */);
  Solution(Solution *s);
  ~Solution();
  void localSearch();
  void build();
  inline double Cost() { return this->cost; }
  void setCost(double cost);
  void setVehicles(int vehicles);
  inline void setSequence(std::vector<std::vector<int>> *sequence);
  static Solution *disturbance(Solution *s);

  double calculateCost();
  bool feasible();
  void resume();

  inline std::vector<std::vector<int>> getSequence() { return this->sequence; }
  inline std::vector<int> getCapacities() { return this->capacities; }
  inline int getVehicles() { return this->vehicles; }
  inline int getDeliveries() { return this->deliveries; }
};

#endif