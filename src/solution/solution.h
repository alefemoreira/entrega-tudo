#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <vector>

class Solution
{
private:
    double cost;
    int vehicles;    
    std::vector<std::vector<int>> *sequence; // (k + 1) x (n + 1)

public:
    Solution(/* args */);
    ~Solution();
    static Solution* build();
    void setCost(double cost);
    void setVehicles(int vehicles);
    void setSequence(std::vector<std::vector<int>> *sequence);
    void resume();
};

Solution::Solution(/* args */)
{
}

Solution::~Solution()
{
}


#endif