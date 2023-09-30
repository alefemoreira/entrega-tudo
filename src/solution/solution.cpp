#include "solution.h"
#include "../reader/reader.h"
#include <iostream>
#include <vector>

using namespace std;

void Solution::build()
{
  this->cost = 0;
  int n = Reader::instance->getDimension();
  int k = Reader::instance->getMAxVehiclesQuantity();
  int l = Reader::instance->getMinimumDelivery();

  int maxVehicles = Reader::instance->getMAxVehiclesQuantity();
  int maxVehicleCapacity = Reader::instance->getCarCapacity();

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
    int better_cost = INFINITY;
    int visited = 0;

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

    this->cost += better_cost;
    if (better_sequence < this->vehicles)
      this->capacities[better_sequence] += better_demand;

    if (visited >= l && k <= this->vehicles)
    {
      k++;
    }
  }
}

Solution::Solution(/* args */)
{
  int k = Reader::instance->getMAxVehiclesQuantity();
  int n = Reader::instance->getDimension();
  this->cost = 0;
  this->vehicles = 0;
  this->sequence = vector<vector<int>>(k + 1, vector<int>());
  this->capacities = vector<int>(k, 0);
}

Solution::~Solution() {}

void Solution::setCost(double cost) { this->cost = cost; }

void Solution::setVehicles(int vehicles) { this->vehicles = vehicles; }

void Solution::setSequence(vector<vector<int>> *sequence)
{
  this->sequence = *sequence;
}

double Solution::getCost(int vi, int j)
{
  if (j == this->vehicles)
  {
    double r = Reader::instance->getCarUseCost();
    double cost = this->capacities[j] == 0 ? r : 0;
    cost += Reader::instance->getOutsourcing(vi);
    return cost;
  }
  else
  {
    int demand = Reader::instance->getDemand(vi);
    int last = this->sequence[j].back();
    int cost = Reader::instance->getDistance(last, vi);
    int maxDemand = Reader::instance->getCarCapacity();

    if (this->capacities[j] + demand >= maxDemand)
    {
      return INFINITY;
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