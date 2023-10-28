#include "reader/reader.h"
#include "solution/solution.h"
#include <chrono>
#include <iomanip>

using namespace std;

int guloso()
{
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  Solution s;
  auto start = std::chrono::high_resolution_clock::now();
  s.build();
  auto end = std::chrono::high_resolution_clock::now();
  duration<double, std::milli> duration_ = end - start;

  cout << Reader::instance->getInstanceName() << ";" << s.Cost() << ";"
       << duration_.count() / 1000 << endl;

  return 0;
}

int vnd()
{
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  Solution s;
  auto start = std::chrono::high_resolution_clock::now();

  s.build();
  s.localSearch();

  auto end = std::chrono::high_resolution_clock::now();
  duration<double, std::milli> duration_ = end - start;

  cout << Reader::instance->getInstanceName() << ";" << s.Cost() << ";"
       << duration_.count() / 1000 << endl;

  // cout << s.calculateCost() << endl;

  return 0;
}

int ils(int maxIter, int maxIterIls)
{
  Solution s0, s1;
  s0.build();
  s0.localSearch();
  cout << "Custo inicial: " << s0.Cost() << endl;

  for (int i = 0; i < maxIterIls; i++)
  {

    int roundsWithoutImprove = 0;
    while (roundsWithoutImprove <= maxIter)
    {
      s1 = Solution::disturbance(&s0);
      s1.localSearch();
      s1.calculateCost();

      // improve?
      if (s0.Cost() <= s1.Cost())
      {
        roundsWithoutImprove += 1;
      }
      else
      {
        s0 = s1;
      }
    }
  }
  cout << "Custo final: " << s0.Cost() << endl;

  // s1 = Solution::disturbance(&s0);

  return 0;
}

int main(int argc, char **argv)
{
  cout << fixed << std::setprecision(5);
  if (argc < 3)
  {
    cout << "Necessário indicar arquivo, opção de execução [guloso : 1, vnd: "
            "2, ils: 3] e seed [opcional]"
         << endl;
    cout << "\n./vin/main instancia op [seed]" << endl;

    return -1;
  }

  Reader::create(argc, argv[1]);
  Reader::instance->read();

  int op = atoi(argv[2]);
  if (argc >= 4)
  {
    int seed = atoi(argv[3]);
    srand(seed);
  }
  else
  {
    srand(time(0));
  }

  switch (op)
  {
  case 1:
    return guloso();

  case 2:
    return vnd();

  case 3:
    return ils(20, 10000);
  default:
    cout << "Opção inválida" << endl;
    return 1;
  }

  //   cout << "Custo recauculado: " << s.calculateCost() << endl;
  //   s.resume();
  //   cout << endl;
  //   cout << endl;
  //   s.vnd();
  //   s.resume();

  //   cout << s.calculateCost() << endl;

  return 0;
}
