#include "iostream"
#include <iterator>
#include <vector>

using namespace std;

struct BestNeighbor
{
    int bestCost;
    vector<vector<int>> sequencies;
};

struct CostSequencies
{
    int costTotal;
    vector<int> costPerSequencie;
};

// Função para calcular o custo da rota
CostSequencies calcCosts(const int &vehicleCost, const vector<vector<int>> &costs, const vector<vector<int>> &sequencies)
{
    CostSequencies costSequencies;
    costSequencies.costTotal = 0;
    int lineCost = 0;
    int sequenciesSize = sequencies.size();
    int currentCity = 0;
    int prevCity = 0;
    int numberCities = 0;
    int firstCity = 0;
    int lastCity = 0;

    for (int line = 0; line < sequenciesSize; line++)
    {
        firstCity = sequencies[line][0];
        lineCost = costs[0][firstCity];
        numberCities = sequencies[line].size();

        for (int i = 0; i < numberCities; i++)
        {
            currentCity = sequencies[line][i];

            if (i > 0)
            {
                prevCity = sequencies[line][i - 1];
                lineCost += costs[prevCity][currentCity];
            }
        }

        lastCity = sequencies[line][numberCities - 1];
        lineCost += costs[lastCity][0];
        lineCost += vehicleCost;
        costSequencies.costPerSequencie.push_back(lineCost);
        costSequencies.costTotal += lineCost;
    }

    return costSequencies;
}

int getCost(const vector<vector<int>> &costs, int i, int j, int numMax)
{
    if (i >= 0 && j < numMax)
    {
        return costs[i][j];
    }

    return 0;
}

int getCostSequence(vector<int> &sequencieCost)
{
    int total = 0;
    vector<int>::iterator it = sequencieCost.begin();

    for (; it < sequencieCost.end(); it++)
    {
        total += *it;
    }

    return total;
}

BestNeighbor swapInLine(const vector<vector<int>> &sequencies, const vector<vector<int>> &costs, CostSequencies currentBestCost, int numMax)
{
    int bestI;
    int bestJ;
    BestNeighbor bestNeighbor;

    for (int line = 0; line < sequencies.size(); line++)
    {
        vector<int> sequencieLine = sequencies[line];
        int citiesQtd = sequencieLine.size();
        vector<int>::iterator currentI;
        vector<int>::iterator currentJ;
        vector<int>::iterator nextI;
        vector<int>::iterator nextJ;
        vector<int>::iterator prevI;
        vector<int>::iterator prevJ;

        for (currentI = sequencieLine.begin(); currentI < sequencieLine.end(); currentI++)
        {
            for (currentJ = currentI + 1; currentJ < sequencieLine.end(); currentJ++)
            {
                nextI = next(currentI);
                prevI = prev(currentI);
                nextJ = next(currentJ);
                prevJ = prev(currentJ);

                int newCost =
                    getCost(costs, *prevI, *currentJ, numMax) +
                    getCost(costs, *currentJ, *nextI, numMax) +
                    getCost(costs, *prevJ, *currentI, numMax) +
                    getCost(costs, *currentI, *nextJ, numMax) -
                    getCost(costs, *prevI, *currentI, numMax) -
                    getCost(costs, *currentI, *nextI, numMax) -
                    getCost(costs, *prevJ, *currentJ, numMax) -
                    getCost(costs, *currentJ, *nextJ, numMax);

                newCost = currentBestCost.costPerSequencie[line] + newCost;

                if (newCost < currentBestCost.costPerSequencie[line])
                {
                    bestI = distance(sequencieLine.begin(), currentI);
                    bestJ = distance(sequencieLine.begin(), currentJ);

                    currentBestCost.costPerSequencie[line] = newCost;
                    int newTotal = getCostSequence(currentBestCost.costPerSequencie);

                    vector<vector<int>> newNeighbor = sequencies;
                    int aux = newNeighbor[line][bestI];
                    newNeighbor[line][bestI] = newNeighbor[line][bestJ];
                    newNeighbor[line][bestJ] = aux;

                    bestNeighbor.bestCost = newTotal;
                    bestNeighbor.sequencies = newNeighbor;

                    return bestNeighbor;
                }
            }
        }
    }

    bestNeighbor.bestCost = currentBestCost.costTotal;
    bestNeighbor.sequencies = sequencies;

    return bestNeighbor;
}

BestNeighbor vnd(const vector<vector<int>> &costs, const vector<vector<int>> &intialSequencie, const CostSequencies initialCosts, int numMax)
{
    vector<vector<int>> currentSequencies = intialSequencie;
    CostSequencies currentBestCost = initialCosts;
    int k = 1; // Índice da vizinhança a ser explorada
    BestNeighbor bestNeighbor;
    bestNeighbor.bestCost = currentBestCost.costTotal;
    bestNeighbor.sequencies = currentSequencies;
    bool better = false;

    while (k <= 1)
    {
        switch (k)
        {
        case 1:
            // Vizinhança 1: Troca a rota da sequência de cidades
            bestNeighbor = swapInLine(currentSequencies, costs, currentBestCost, numMax);
            break;

        default:
            break;
        }

        better = false;

        if (bestNeighbor.bestCost < currentBestCost.costTotal)
        {
            currentSequencies = bestNeighbor.sequencies;
            currentBestCost.costTotal = bestNeighbor.bestCost;
            better = true;
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

    BestNeighbor newBestNeighbor;
    newBestNeighbor.sequencies = currentSequencies;
    newBestNeighbor.bestCost = currentBestCost.costTotal;
    return newBestNeighbor;
}

int main(int argc, char const *argv[])
{
    // Exemplo de uso
    int r = 2;
    int n = 6;

    // Tempos de preparo entre produtos (exemplo fictício)
    vector<vector<int>> costs = {
        {0, 2, 3, 4, 1, 9, 6},
        {2, 0, 3, 2, 3, 2, 9},
        {5, 5, 0, 5, 13, 7, 5},
        {7, 3, 1, 0, 3, 4, 11},
        {6, 5, 3, 1, 0, 4, 15},
        {2, 8, 11, 2, 3, 0, 8},
        {7, 10, 7, 10, 15, 8, 0}};

    // Sequência inicial de produtos em cada linha de produção
    vector<vector<int>> initialSequences = {
        {1, 2},
        {3, 4, 5}};

    // Imprime a sequência inicial de produtos
    cout << "INITIAL SEQUENCE:" << endl;
    for (int Line = 0; Line < initialSequences.size(); Line++)
    {
        for (int city : initialSequences[Line])
        {
            cout << " " << city;
        }
        cout << endl;
    }

    // Calcula o tempo incial de operação da fábrica para a sequência inicial
    CostSequencies costSequencies = calcCosts(r, costs, initialSequences);
    cout << "Tempo total de operação da fábrica com sequencia inicial: " << costSequencies.costTotal << endl;

    cout << '\n'
         << endl;

    // Aplica o algoritmo VND para obter a melhor sequência de produtos
    BestNeighbor bestNeighbor = vnd(costs, initialSequences, costSequencies, n);

    // Imprime a melhor sequência de produtos
    cout << "Melhor sequência:" << endl;
    for (int line = 0; line < bestNeighbor.sequencies.size(); line++)
    {
        for (int city : bestNeighbor.sequencies[line])
        {
            cout << city << " ";
        }
        cout << endl;
    }

    // Calcula o tempo total de operação da fábrica para a melhor sequência
    cout << "Total vnd " << bestNeighbor.bestCost << endl;

    return 0;
}
