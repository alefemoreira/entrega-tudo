#include "reader/reader.h"
#include "solution/solution.h"

using namespace std;

int main(int argc, char **argv) {
  if (argc < 3) {
    cout << "Necessário indicar arquivo e opção de execução [1, 2, 3]" << endl;
    return 1;
  }

  Reader::create(argc, argv[1]);
  Reader::instance->read();
  Solution s;
  int op = atoi(argv[2]);

  switch (op) {
  case 1:
    s.build();
    cout << "Custo guloso: " << s.Cost() << endl;
    break;

  case 2:
    s.build();
    s.vnd();
    cout << "Custo vnd: " << s.Cost() << endl;
    break;

  case 3:
    cout << "ILS ainda não implementado" << endl;
    break;

  default:
    cout << "Opção inválida" << endl;
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
