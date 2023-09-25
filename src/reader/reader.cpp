#include "reader.h"
#include <fstream>
#include <iostream>

using namespace std;

Reader::Reader(std::string path) { this->instancePath = path; }
Reader::~Reader() {}

Reader *Reader::instance = nullptr;

void Reader::create(std::string path) {
  if (instance == nullptr)
    Reader::instance = new Reader(path);
}

void Reader::read() { 
    ifstream in(this->instancePath, ios::in);
    string file;
    
    if(!in) {
        cout << "File not found";
        exit(1);
    }

    // if

    // this->distances = new double*[n]
 }