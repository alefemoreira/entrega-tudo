#ifndef READER_H
#define READER_H

#include <iostream>

class Reader {
private:
  static Reader *instance;
  int dimension;     // n
  int maxVehicles;   // k
  int carCapacity;   // Q
  int minDelivery;   // L
  double carUseCost; // r

  int *demands;
  int *outsourcing;
  double **distances;

public:
  Reader();
  ~Reader();

  double getDistante(int i, int j) { return this->distances[i][j]; }
  int getDemand(int i) { return this->demands[i]; }
  int getOutsourcing(int i) { return this->outsourcing[i]; }
  int getCarCapacity() { return this->carCapacity; }
  int getDimension() { return this->dimension; }
  int getMAxVehiclesQuantity() { return this->maxVehicles; }
  double getCarUseCost() { return this->carUseCost; }
  int getMinimumDelivery() { return this->minDelivery; }
};

Reader::Reader() {}

Reader::~Reader() {}

#endif