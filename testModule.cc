#include "doubleArray.h"
//#include "initializeT.h"
//#include "writeFile.h"
//#include "averageT.h"
//#include "updateTserial.h"
#include <stdio.h>

int main() {
  int nx = 10;
  doubleArray T = allocateArray(nx);
  freeArray(T, nx);
  printf("doubleArray.h Success!\n");

  // int nx = 10;
  // double dx = 0.1;
  // doubleArray T = allocateArray(nx);
  // initializeTemperature(T, nx, dx);
  // printf("initializeT.h Success!\n");

  // int nx = 10;
  // doubleArray T = allocateArray(nx);
  // writerFile("fileName", T, nx);
  // printf("writeFile.h Success!\n");

  // int nx = 10;
  // doubleArray T = allocateArray(nx);
  // averageTemperature(T, nx);
  // printf("averageT.h Success!\n");

  // int nx = 10;
  // doubleArray T = allocateArray(nx);
  // doubleArray Told = allocateArray(nx);
  // updateTemperatureSerial(Told, T, 1, 1, 1, 1);
  // printf("updateTserial.h Success!\n");

  return 0;
}
