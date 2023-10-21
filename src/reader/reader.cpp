#include "reader.h"
#include <fstream>
#include <iostream>

using namespace std;

Reader::Reader(int count, char *path) {
  this->argCount = count;
  this->instancePath = path;
  this->instanceName = path;
}
Reader::~Reader() {}

Reader *Reader::instance = nullptr;

void Reader::create(int count, char *path) {
  if (instance == nullptr)
    Reader::instance = new Reader(count, path);
}

string Reader::getInstanceName() {
  string::size_type loc = instanceName.find_last_of(".", instanceName.size());
  string::size_type loc2 = instanceName.find_last_of("/", instanceName.size());

  string instance;

  if (loc != string::npos) {
    instance.append(instanceName, loc2 + 1, loc - loc2 - 1);
  } else {
    instance.append(instanceName, loc2 + 1, instance.size());
  }

  return instance;
}

void Reader::read() {
  ifstream instanceFile(this->instancePath, ios::in);
  string file;

  if (!instanceFile) {
    cout << "File not found" << endl;
    exit(1);
  }

  instanceFile >> this->dimension;
  instanceFile >> this->maxVehicles;
  instanceFile >> this->carCapacity;
  instanceFile >> this->minDelivery;
  instanceFile >> this->carUseCost;

  this->demands = new int[this->dimension];
  this->outsourcing = new double[this->dimension];
  this->distances = new double *[this->dimension + 1];
  for (int i = 0; i < dimension + 1; i++) {
    this->distances[i] = new double[dimension + 1];
  }

  for (int i = 0; i < dimension; i++) {
    instanceFile >> this->demands[i];
  }

  for (int i = 0; i < dimension; i++) {
    instanceFile >> this->outsourcing[i];
  }

  for (int i = 0; i < dimension + 1; i++) {
    for (int j = 0; j < dimension + 1; j++) {
      instanceFile >> this->distances[i][j];
      if (i == j)
        this->distances[i][j] = 0;
    }
  }

  instanceFile.close();
  // this->distances = new double*[n]
}

void Reader::showInstance() {
  cout << "Entregas: " << this->dimension << endl;
  cout << "Veículos: " << this->maxVehicles << endl;
  cout << "Capacidade dos veículos: " << this->carCapacity << endl;
  cout << "Quantidade mínima de entrega: " << this->minDelivery << endl;
  cout << "Custo por uso do carro: " << this->carUseCost << endl;
  cout << endl << endl;

  cout << "Demandas: " << endl;
  for (int i = 0; i < dimension; i++) {
    cout << this->demands[i] << " ";
  }
  cout << endl << endl;

  cout << "Custo da terceirização" << endl;
  for (int i = 0; i < dimension; i++) {
    cout << this->outsourcing[i] << " ";
  }
  cout << endl << endl;

  cout << "Matriz de distâncias: " << endl;

  for (int i = 0; i < dimension + 1; i++) {
    for (int j = 0; j < dimension + 1; j++) {
      cout << this->distances[i][j] << " ";
    }
    cout << endl;
  }
}