#include "updateTomp.h"
#include <omp.h>

int updateTemperatureOMP(doubleArray Told, doubleArray T, const double k,
                         const int nx, const double dx, const double dt) {
  int i, j;
  double laplaceT = 0;
  // x=0, x=pi periodic boundary condition
  for (j = 1; j < nx - 1; j++) {
    // x=0
    laplaceT = (Told[nx - 1][j] + Told[1][j] + Told[0][j - 1] + Told[0][j + 1] -
                4 * Told[0][j]) /
               (dx * dx);
    T[0][j] = Told[0][j] + dt * k * laplaceT;
    // x= pi
    laplaceT = (Told[nx - 2][j] + Told[0][j] + Told[nx - 1][j - 1] +
                Told[nx - 1][j + 1] - 4 * Told[nx - 1][j]) /
               (dx * dx);
    T[nx - 1][j] = Told[nx - 1][j] + dt * k * laplaceT;
  }

#pragma omp parallel for
  // update inner points [1,nx-1], [1,nx-1]
  for (i = 1; i < nx - 1; i++) {
    for (j = 1; j < nx - 1; j++) {
      // center difference
      laplaceT = (Told[i - 1][j] + Told[i + 1][j] + Told[i][j - 1] +
                  Told[i][j + 1] - 4 * Told[i][j]) /
                 (dx * dx);
      // forward euler
      T[i][j] = Told[i][j] + dt * k * laplaceT;
    }
  }

  // update inner temporature field
  for (i = 0; i < nx; i++) {
    for (j = 1; j < nx - 1; j++) {
      Told[i][j] = T[i][j];
    }
  }

  return 0;
}
