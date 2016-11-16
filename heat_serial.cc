#include "averageT.h"
#include "doubleArray.h"
#include "initializeT.h"
#include "updateTserial.h"
#include "writeFile.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[]) {
  // input check
  if (argc != 2) {
    printf("USAGE: %s <nx>\n", argv[0]);
    exit(1);
  }
  const int nx = atoi(argv[1]);

  // set parameters for heat equation
  const double k = 1;
  const double PI = 2 * acos(0);
  const double t = 0.5 * PI * PI / k;
  const double dx = PI / nx;
  // for numerical stability, dt < dx^2/(4k)
  const double dt = 0.5 * dx * dx / (4 * k);
  const int nt = (int)t / dt;
  printf("k=%f,x=%f,t=%f,nx=%d,nt=%d,dx=%f,dt=%f\n", k, PI, t, nx, nt, dx, dt);

  // time start
  clock_t start = clock();

  // allocate memory
  doubleArray T = allocateArray(nx);
  doubleArray Told = allocateArray(nx);

  // initialize Temperature
  initializeTemperature(T, nx, dx);
  initializeTemperature(Told, nx, dx);

  // solve heat equation
  for (int n = 0; n < nt; n++) {
    updateTemperatureSerial(Told, T, k, nx, dx, dt);
  }

  // average Temperature
  double aveTemp = averageTemperature(T, nx);
  printf("Volume average temperature = %.4f\n", aveTemp);

  // write final temperature to files
  char fileName[50];
  sprintf(fileName, "heat_serial_%d.dat", nx);
  // string fileName = "Temperature.dat";
  writerFile(fileName, T, nx);

  // free memroy
  freeArray(Told, nx);
  freeArray(T, nx);

  // time stop
  clock_t stop = clock();
  double elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
  printf("Time elapsed in seconds: %.4f\n", elapsed);

  // end the program
  return 0;
}
