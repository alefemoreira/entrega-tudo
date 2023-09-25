#ifndef READER_H
#define READER_H

#include <iostream>
#include <string>

class Reader {
private:
  int dimension;     // n
  int maxVehicles;   // k
  int carCapacity;   // Q
  int minDelivery;   // L
  double carUseCost; // r

  int *demands;
  int *outsourcing;
  double **distances;

  std::string instancePath;

public:
  Reader(std::string path);
  ~Reader();

  static Reader *instance;

  static void create(std::string path);

  void read();

  inline double getDistante(int i, int j) { return this->distances[i][j]; }
  inline int getDemand(int i) { return this->demands[i]; }
  inline int getOutsourcing(int i) { return this->outsourcing[i]; }
  inline int getCarCapacity() { return this->carCapacity; }
  inline int getDimension() { return this->dimension; }
  inline int getMAxVehiclesQuantity() { return this->maxVehicles; }
  inline double getCarUseCost() { return this->carUseCost; }
  inline int getMinimumDelivery() { return this->minDelivery; }
};

#endif