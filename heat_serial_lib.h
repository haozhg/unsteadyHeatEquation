#ifndef HEAT_SERIAL_LILB
#define HEAT_SERIAL_LILB

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

typedef double **doubleArray;

doubleArray allocateArray(int nx);

int freeArray(doubleArray a, int nx);

int initializeTemperature(doubleArray T, const int nx, const double dx);

int updateTemperature(doubleArray Told, doubleArray T, const double k,
                      const int nx, const double dx, const double dt);

int solveTemperature(doubleArray Told, doubleArray T, const double k,
                     const int nx, const int nt, const double dx,
                     const double dt);

double averageTemperature(doubleArray T, const int nx);

int writerFile(string fileName, doubleArray T, int nx);

#endif // HEAT_SERIAL_LILB
