#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <vector>

class Solution {
private:
  double cost;    // custo da solução
  int vehicles;   // qtd de vehicles usada pela solução
  int deliveries; // qtd de entregas ñ terceirizadas da solução
  std::vector<std::vector<int>> sequence; // (k + 1) x (n + 2)
  std::vector<int>
      capacities; // quanto da capacidade de cada veículo é utilizada;
  double getCost(int i, int j, double d);

  bool bestImprovementReinsertionVehicles();
  bool bestImprovementOutsourcing();
  bool bestImprovementUndoOutsourcing();
  bool bestImprovementSwapVehicles();

  void reinsertion(int k, int i, int l, int j);
  void peformsReinsertionOutsourcing(int k, int i);

  /*
    Remove elemento outsouring[i] e insere em sequence[k][j];
    O(n)
  */
  void peformsRemovalOutsourcing(int i, int k, int j);
  void swap(int k1, int i, int k2, int j);

  void swapElementsBetweenArrays(int k1, int i, int k2, int j);

public:
  Solution(/* args */);
  ~Solution();
  void vnd();
  void build();
  void setCost(double cost);
  void setVehicles(int vehicles);
  inline void setSequence(std::vector<std::vector<int>> *sequence);

  double calculateCost();
  void resume();
};

#endif