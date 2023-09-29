#include "iostream"
#include <vector>

using namespace std;

struct BestNeighbor
{
    int bestCost;
    vector<vector<int>> sequencies;
};

// Função para calcular o custo da rota
int calcCosts(const int &vehicleCost, const vector<vector<int>> &costs, const vector<vector<int>> &sequencies)
{
    int lineCost = 0;
    int sequenciesSize = sequencies.size();

    for (int line = 0; line < sequenciesSize; line++)
    {
        int numberCities = sequencies[line].size();

        for (int i = 0; i < numberCities; i++)
        {
            int currentCitie = sequencies[line][i];

            if (i > 0)
            {
                int prevCitie = sequencies[line][i - 1];
                lineCost += costs[prevCitie - 1][currentCitie - 1];
            }
        }
    }

    lineCost += vehicleCost;

    return lineCost;
}

BestNeighbor swapInLine(const int &vehicleCost, const vector<vector<int>> &sequencies, const vector<vector<int>> &costs)
{
    for (int line = 0; line < sequencies.size(); line++)
    {
        vector<int> sequencieLine = sequencies[line];
        int citiesQtd = sequencieLine.size();

        for (int i = 0; i < citiesQtd; i++)
        {
            for (int j = i + 1; j < citiesQtd; j++)
            {
                        }
        }
    }
}

vector<vector<int>> vnd(const int &vehicleCost, const vector<vector<int>> &costs, const vector<vector<int>> &intialSequencie)
{
    vector<vector<int>> currentSequencie = intialSequencie;
    int currentBestCost = calcCosts(vehicleCost, costs, intialSequencie);
    int k = 1; // Índice da vizinhança a ser explorada
    BestNeighbor bestNeighbor;
    bestNeighbor.bestCost = currentBestCost;
    bestNeighbor.sequencies = currentSequencie;
    bool better = false;

    while (k <= 1)
    {
        switch (k)
        {
        case 1:
            // Vizinhança 1: Troca de posição de dois produtos em uma linha de produção
            // bestNeighbor = buscaLocalNaLinha(tempoProdutos, sequenciaAtual, tempos);
            break;

        default:
            break;
        }

        better = false;

        if (bestNeighbor.bestCost < currentBestCost)
        {
            currentSequencie = bestNeighbor.sequencies;
            currentBestCost = bestNeighbor.bestCost;
            better = true;
            break; // Melhorou, então para de explorar a vizinhança atual
        }

        if (!better)
        {
            k++; // Passa para a próxima vizinhança
        }
        else
        {
            k = 1; // Volta para a primeira vizinhança
        }
    }

    return currentSequencie;
}

int main(int argc, char const *argv[])
{

    return 0;
}
