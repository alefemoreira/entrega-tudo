#include <iostream>
#include "solution.h"

using namespace std;

Solution *Solution::build()
{

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
    this->sequence = sequence;
}

void Solution::resume()
{
    cout << "Custo: " << this->cost << endl;
    cout << "Número de veiculos: " << this->vehicles << endl;
    for (size_t i = 0; i < this->sequence->size(); i++)
    {
        if (i != this->sequence->size())
        {
            cout << "Rota do veiculo : " << i + 1 << endl;
        }
        else
        {
            cout << "Pontos de entrega tercerizados: " << endl;
        }

        for (size_t j = 0; j < this->sequence->size(); j++)
        {
            cout << " " << (*sequence)[i][j] << " ";
        }
        cout << endl;
    }
}