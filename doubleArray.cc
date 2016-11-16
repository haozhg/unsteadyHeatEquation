#include "doubleArray.h"

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
