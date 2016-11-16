#ifndef DOUBLEARRAY_H_
#define DOUBLEARRAY_H_

typedef double **doubleArray;

doubleArray allocateArray(int nx);

int freeArray(doubleArray a, int nx);

#endif // DOUBLEARRAY_H_
