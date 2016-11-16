#include "initializeT.h"
#include <cmath>

int initializeTemperature(doubleArray T, const int nx, const double dx) {
  int i, j;
  // initial condition, boundary condition
  for (i = 0; i < nx; i++) {
    for (j = 0; j < nx; j++) {
      if (j == 0) {
        T[i][j] = cos(i * dx) * cos(i * dx);
      } else if (j == nx - 1) {
        T[i][j] = sin(i * dx) * sin(i * dx);
      } else {
        T[i][j] = 0;
      }
    }
  }
  return 0;
}
