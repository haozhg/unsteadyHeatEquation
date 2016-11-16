#include "averageT.h"

double averageTemperature(doubleArray T, const int nx) {
  double aveT;
  double sum = 0;
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < nx; j++) {
      sum += T[i][j];
    }
  }
  aveT = sum / (nx * nx);
  return aveT;
}
