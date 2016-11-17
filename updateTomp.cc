#include "updateTomp.h"
#include <omp.h>
#include <stdio.h>

int updateTemperatureOMP(doubleArray current, doubleArray next, const double k,
                         const int nx, const double dx, const double dt) {
  int i, j;
  double laplaceT;
#pragma omp parallel for // shared(current, next) private(i, j)
  // update inner points [0,nx-1], [1,nx-2]
  for (i = 0; i < nx; i++) {
    for (j = 1; j < nx - 1; j++) {
      if (i == 0) {
        // x=0
        laplaceT = (current[nx - 1][j] + current[i + 1][j] + current[i][j - 1] +
                    current[i][j + 1] - 4 * current[i][j]) /
                   (dx * dx);
      } else if (i == nx - 1) {
        // x= pi
        laplaceT = (current[i - 1][j] + current[0][j] + current[i][j - 1] +
                    current[i][j + 1] - 4 * current[i][j]) /
                   (dx * dx);
      } else {
        // center difference
        laplaceT = (current[i - 1][j] + current[i + 1][j] + current[i][j - 1] +
                    current[i][j + 1] - 4 * current[i][j]) /
                   (dx * dx);
      }
      // forward euler
      next[i][j] = current[i][j] + dt * k * laplaceT;
    }
  }
  return 0;
}
