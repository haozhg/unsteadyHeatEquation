#ifndef HEAT_SERIAL_LIB_H_
#define HEAT_SERIAL_LIB_H_
#include <string>
using namespace std;

typedef double **doubleArray;

doubleArray allocateArray(int nx);

int freeArray(doubleArray a, int nx);

int initializeTemperature(doubleArray T, const int nx, const double dx);

int updateTemperatureSerial(doubleArray Told, doubleArray T, const double k,
                            const int nx, const double dx, const double dt);

double averageTemperature(doubleArray T, const int nx);

int writerFile(string fileName, doubleArray T, int nx);

#endif // HEAT_SERIAL_LIB_H_
