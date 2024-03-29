#ifndef READER_H
#define READER_H

#include <iostream>
#include <string>

#define INFINITE 99999999

class Reader {
private:
  int dimension;     // n
  int maxVehicles;   // k
  int carCapacity;   // Q
  int minDelivery;   // L
  double carUseCost; // r

  int *demands;
  double *outsourcing;
  double **distances;

  char *instancePath;
  int argCount;
  std::string instanceName;

public:
  Reader(int count, char *path);
  ~Reader();

  static Reader *instance;

  static void create(int count, char *path);

  void read();
  void showInstance();
  std::string getInstanceName();

  inline double getDistance(int i, int j) { return this->distances[i][j]; }
  inline int getDemand(int i) { return this->demands[i - 1]; }
  inline double getOutsourcing(int i) { return this->outsourcing[i - 1]; }
  inline int getCarCapacity() { return this->carCapacity; }
  inline int getDimension() { return this->dimension; }
  inline int getMaxVehiclesQuantity() { return this->maxVehicles; }
  inline double getCarUseCost() { return this->carUseCost; }
  inline int getMinimumDelivery() { return this->minDelivery; }
};

#endif