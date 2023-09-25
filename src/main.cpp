#include "reader/reader.h"
#include "solution/solution.h"

using namespace std;

int main(int argc, char const *argv[])
{
    cout << argc << endl;
    Reader::create(argv[2]);
    Reader::instance->read();
    return 0;
}
