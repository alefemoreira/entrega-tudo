#include "solution.h"
#include "../reader/reader.h"
#include <iostream>
#include <vector>

using namespace std;

void Solution::build()
{
  int maxVehicles = Reader::instance->getMAxVehiclesQuantity();
  if (maxVehicles) // adicionado apenas para evitar warnings. Remova ao implementar
    return;
}

Solution::Solution(/* args */)
{
  int k = Reader::instance->getMAxVehiclesQuantity();
  int n = Reader::instance->getDimension();
  this->cost = 0;
  this->vehicles = 0;
  this->sequence = vector<vector<int>>(k + 1, vector<int>(0, n + 2));
  this->capacities = vector<int>(k, 0);
}

Solution::~Solution() {}

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