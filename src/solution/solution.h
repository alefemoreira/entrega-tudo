#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <vector>

class Solution
{
private:
    double cost;
    int vehicles;
    std::vector<std::vector<int>> sequence; // (k + 1) x (n + 2)
    std::vector<int> capacities;            // quanto da capacidade de cada veículo voi utilizada;

    void bestImprovementReinsertionVehicles();
    void reinsertion(int k1, int i, int k2, int j);
public:
    Solution(/* args */);
    ~Solution();
    void build();
    void setCost(double cost);
    void setVehicles(int vehicles);
    inline void setSequence(std::vector<std::vector<int>> *sequence);
    void resume();
};

#endif