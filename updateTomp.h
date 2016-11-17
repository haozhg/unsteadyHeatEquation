#ifndef UPDATETSERIAL_H_
#define UPDATETSERIAL_H_
#include "doubleArray.h"

int updateTemperatureOMP(doubleArray Told, doubleArray T, const double k,
                         const int nx, const double dx, const double dt,
                         const int nthreads);

#endif // UPDATETSERIAL_H_
