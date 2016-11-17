#include "averageT.h"
#include "doubleArray.h"
#include "initializeT.h"
#include "updateTomp.h"
#include "writeFile.h"
#include <cmath>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[]) {
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
  printf("k=%f, x=%f, t=%f, nx=%d, nt=%d, dx=%f, dt=%f\n", k, PI, t, nx, nt, dx,
         dt);

  // time start
  clock_t start = clock();
  // double start = omp_get_wtime();

  // allocate memory
  doubleArray next = allocateArray(nx);
  doubleArray current = allocateArray(nx);

  // initialize Temperature
  initializeTemperature(next, nx, dx);
  initializeTemperature(current, nx, dx);

  // solve heat equation
  // omp_set_dynamic(0);
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
  sprintf(fileName, "heat_omp_%d_%d.dat", nx, nthreads);
  writerFile(fileName, next, nx);

  // free memroy
  freeArray(current, nx);
  freeArray(next, nx);

  // time stop
  clock_t stop = clock();
  double elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
  printf("nthreads = %d, nx= %d, time [seconds] = %.4f\n", nthreads, nx,
         elapsed);
  // printf("nthreads = %d, nx= %d, time [seconds] = %.4f\n", nthreads, nx,
  //        omp_get_wtime() - start);

  // end the program
  return 0;
}
