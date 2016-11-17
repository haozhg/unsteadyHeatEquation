#include "updateTomp.h"
#include <omp.h>
#include <stdio.h>

int updateTemperatureOMP(doubleArray Told, doubleArray T, const double k,
                         const int nx, const double dx, const double dt,
                         const int nthreads) {
  int i, j;
  double laplaceT = 0;

  omp_set_num_threads(nthreads); // Number of threads
#pragma omp parallel shared(Told, T, laplaceT) private(i, j)
  // update inner points i \in [0,nx-1], j \in [1,nx-2]
  for (i = 0; i < nx; i++) {
    for (j = 1; j < nx - 1; j++) {
      if (i == 0) {
        // left boundary, x=0
        laplaceT = (Told[nx - 1][j] + Told[1][j] + Told[0][j - 1] +
                    Told[0][j + 1] - 4 * Told[0][j]) /
                   (dx * dx);
      } else if (i == nx - 1) {
        // right boundary, x= pi
        laplaceT = (Told[nx - 2][j] + Told[0][j] + Told[nx - 1][j - 1] +
                    Told[nx - 1][j + 1] - 4 * Told[nx - 1][j]) /
                   (dx * dx);
      } else {
        // center difference
        laplaceT = (Told[i - 1][j] + Told[i + 1][j] + Told[i][j - 1] +
                    Told[i][j + 1] - 4 * Told[i][j]) /
                   (dx * dx);
      }
      // forward euler
      T[i][j] = Told[i][j] + dt * k * laplaceT;
    }
  }
  return 0;
}
