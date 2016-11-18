#include "heat_serial_lib.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdlib.h>

doubleArray allocateArray(int nx) {
  doubleArray T = new double *[nx];
  for (int i = 0; i < nx; i++) {
    T[i] = new double[nx];
  }
  return T;
}

int freeArray(doubleArray array, int nx) {
  for (int i = 0; i < nx; i++) {
    delete[] array[i];
  }
  delete[] array;
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

int updateTemperatureSerial(doubleArray current, doubleArray next,
                            const double k, const int nx, const double dx,
                            const double dt) {
  int i, j;
  double laplaceT = 0;
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
