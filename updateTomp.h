#ifndef UPDATETSERIAL_H_
#define UPDATETSERIAL_H_
#include "doubleArray.h"

int updateTemperatureOMP(doubleArray current, doubleArray next, const double k,
                         const int nx, const double dx, const double dt);

#endif // UPDATETSERIAL_H_
