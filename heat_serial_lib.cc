#include "heat_serial_lib.h"

doubleArray allocateArray(int nx) {
  doubleArray T = new double *[nx];
  for (int i = 0; i < nx; i++) {
    T[i] = new double[nx];
  }
  return T;
}

int freeArray(doubleArray a, int nx) {
  for (int i = 0; i < nx; i++) {
    delete[] a[i];
  }
  delete[] a;
  return 0;
}

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

int updateTemperature(doubleArray Told, doubleArray T, const double k,
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

int solveTemperature(doubleArray Told, doubleArray T, const double k,
                     const int nx, const int nt, const double dx,
                     const double dt) {
  for (int n = 0; n < nt; n++) {
    updateTemperature(Told, T, k, nx, dx, dt);
  }
  return 0;
}

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

int writerFile(string fileName, doubleArray T, int nx) {
  int i, j;
  ofstream output(fileName);
  if (output.is_open()) {
    for (j = 0; j < nx; j++) {
      for (i = 0; i < nx; i++) {
        if (i < nx - 1) {
          output << T[i][j] << ", ";
        } else {
          output << T[i][j];
        }
      }
      output << "\n";
    }
    output.close();
  } else {
    cout << "Unable to open file!\n";
  }
  return 0;
}
