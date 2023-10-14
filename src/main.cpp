#include "reader/reader.h"
#include "solution/solution.h"

using namespace std;

int main(int argc, char **argv) {
  Reader::create(argc, argv[1]);
  Reader::instance->read();
  Solution s;
  s.build();
  s.resume();
  cout << endl;
  cout << endl;
  s.vnd();
  s.resume();

  cout << "\nCusto: ";
  cout << s.calculateCost() << endl;

  return 0;
}
