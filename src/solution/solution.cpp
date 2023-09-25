#include <iostream>
#include "solution.h"
#include "../reader/reader.h"

using namespace std;

Solution *Solution::build()
{
    Solution *solution = new Solution();
    int maxVehicles = Reader::instance->getMAxVehiclesQuantity();

    return solution;
}

void Solution::setCost(double cost)
{
    this->cost = cost;
}

void Solution::setVehicles(int vehicles)
{
    this->vehicles = vehicles;
}

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
            cout << "Rota do veiculo : " << i + 1 << endl;
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