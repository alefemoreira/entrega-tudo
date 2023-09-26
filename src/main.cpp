#include "reader/reader.h"
#include "solution/solution.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Reader::create(argc, argv[2]);
    Reader::instance->read();
    return 0;
}
