#include "reader.h"
#include <fstream>
#include <iostream>

using namespace std;

Reader::Reader(int count, std::string path)
{
  this->argCount = count;
  this->instancePath = path;
}
Reader::~Reader() {}

Reader *Reader::instance = nullptr;

void Reader::create(int count, std::string path)
{
  if (instance == nullptr)
    Reader::instance = new Reader(count, path);
}

void Reader::read()
{
  ifstream in(this->instancePath, ios::in);
  string file;

  if (!in)
  {
    cout << "File not found" << endl;
    exit(1);
  }

  // if

  // this->distances = new double*[n]
}