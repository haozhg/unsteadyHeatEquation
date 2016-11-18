#include <cmath>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

typedef double **doubleArray;

doubleArray allocateArray(const int nx) {
  doubleArray T = new double *[nx];
  for (int i = 0; i < nx; i++) {
    T[i] = new double[nx];
  }
  return T;
}

int freeArray(doubleArray array, const int nx) {
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

int updateTemperatureOMP(doubleArray current, doubleArray next, const double k,
                         const int nx, const double dx, const double dt) {
  // int i, j;
  double laplaceT = 0;
// int th_id = 0;

// OpenMP parallel
#pragma omp parallel for
  // update inner points [0,nx-1], [1,nx-2]
  for (int i = 0; i < nx; i++) {
    // printf("Thread #%d is doing column %d.\n", th_id, j);
    for (int j = 1; j < nx - 1; j++) {
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

int writeFile(string fileName, doubleArray T, const int nx) {
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

int main(int argc, char *argv[]) {
  // time start
  clock_t start = clock();
  // double start = omp_get_wtime();

  // input check
  if (argc != 3) {
    printf("USAGE: %s <nx> <nthreads>\n", argv[0]);
    exit(1);
  }
  const int nx = atoi(argv[1]);
  const int nthreads = atoi(argv[2]);

  // set parameters for heat equation
  const double k = 1;
  const double PI = 2 * acos(0);
  const double t = 0.5 * PI * PI / k;
  const double dx = PI / nx;
  // for numerical stability, dt < dx^2/(4k)
  const double dt = 0.5 * dx * dx / (4 * k);
  const int nt = (int)t / dt;
  printf("nthreads=%d, k=%f, x=%f, t=%f, nx=%d, nt=%d, dx=%f, dt=%f\n",
         nthreads, k, PI, t, nx, nt, dx, dt);

  // declare array, and allocate memory
  doubleArray next = allocateArray(nx);
  doubleArray current = allocateArray(nx);

  // initialize Temperature
  initializeTemperature(next, nx, dx);
  initializeTemperature(current, nx, dx);

  // solve heat equation
  omp_set_num_threads(nthreads); // set the number of threads
  doubleArray ptrTemp;
  for (int n = 0; n < nt; n++) {
    updateTemperatureOMP(current, next, k, nx, dx, dt);
    // switch old and new Temperature
    ptrTemp = current;
    current = next;
    next = ptrTemp;
  }

  // average Temperature
  double aveTemp = averageTemperature(next, nx);
  printf("Average temperature = %.4f\n", aveTemp);

  // write final temperature to files
  char fileName[50];
  sprintf(fileName, "heat_omp_nx%d_nth%d.dat", nx, nthreads);
  writeFile(fileName, next, nx);

  // free memroy
  freeArray(current, nx);
  freeArray(next, nx);

  // time stop
  clock_t stop = clock();
  double elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
  printf("nthreads = %d, nx = %d, time [seconds] = %.4f\n", nthreads, nx,
         elapsed);
  // printf("nthreads = %d, nx= %d, time [seconds] = %.4f\n", nthreads, nx,
  //        omp_get_wtime() - start);

  // end the program
  return 0;
}
