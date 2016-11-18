#include <cmath>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

typedef double **doubleArray;

doubleArray allocateArrayMPI(const int nproc, const int nx) {
  doubleArray T = new double *[nproc];
  for (int i = 0; i < nproc; i++) {
    T[i] = new double[nx];
  }
  return T;
}

int freeArrayMPI(doubleArray array, const int nproc, const int nx) {
  for (int i = 0; i < nproc; i++) {
    delete[] array[i];
  }
  delete[] array;
  return 0;
}

int initializeTemperatureMPI(doubleArray T, const int nproc, const int nx,
                             const double xproc, const double dx) {
  int i, j;
  double x = xproc + i * dx;
  // initial condition, boundary condition
  for (i = 0; i < nproc; i++) {

    for (j = 0; j < nx; j++) {
      if (j == 0) {
        T[i][j] = cos(x) * cos(x);
      } else if (j == nx - 1) {
        T[i][j] = sin(x) * sin(x);
      } else {
        T[i][j] = 0;
      }
    }
  }
  return 0;
}

int updateTemperatureMPI(doubleArray current, doubleArray next, const double k,
                         const int nproc, const int nx, const double dx,
                         const double dt, const double *left,
                         const double *right) {
  int i, j;
  double laplaceT = 0;
  // update inner points [0,nx-1], [1,nx-2]
  for (i = 0; i < nproc; i++) {
    for (j = 1; j < nx - 1; j++) {
      if (i == 0) {
        // x=0
        laplaceT = (left[j] + current[i + 1][j] + current[i][j - 1] +
                    current[i][j + 1] - 4 * current[i][j]) /
                   (dx * dx);
      } else if (i == nproc - 1) {
        // x= pi
        laplaceT = (current[i - 1][j] + right[j] + current[i][j - 1] +
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

double averageTemperatureMPI(doubleArray T, const int nproc, const int nx) {
  double aveT;
  double sum = 0;
  for (int i = 0; i < nproc; i++) {
    for (int j = 0; j < nx; j++) {
      sum += T[i][j];
    }
  }
  aveT = sum / (nproc * nx);
  return aveT;
}

int writeFileMPI(string fileName, doubleArray T, const int nproc,
                 const int nx) {
  int i, j;
  ofstream output(fileName);
  if (output.is_open()) {
    for (j = 0; j < nproc; j++) {
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
  printf("k=%f, x=%f, t=%f, nx=%d, nt=%d, dx=%f, dt=%f\n", k, PI, t, nx, nt, dx,
         dt);

  // declare parameters for MPI process
  int size, rank, before, after, rc, nproc;
  double xproc, averageT;
  MPI_Status stats;
  double left[nx], right[nx], leftSend[nx], rightSend[nx];

  // MPI parallel
  rc = MPI_Init(&argc, &argv);
  if (rc != MPI_SUCCESS) {
    cout << "Can't not start MPI" << endl;
    MPI_Abort(MPI_COMM_WORLD, rc);
  }
  // rank and total size of processors
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // send and receive rank
  before = rank - 1;
  after = rank + 1;
  if (rank == 0) {
    before = size - 1;
  }
  if (rank == size - 1) {
    after = 0;
  }

  // initialize arrays, allocate memory
  nproc = nx / size;
  doubleArray next = allocateArrayMPI(nproc, nx);
  doubleArray current = allocateArrayMPI(nproc, nx);

  // initialize Temperature
  xproc = rank * PI / size;
  initializeTemperatureMPI(next, nproc, nx, xproc, dx);
  initializeTemperatureMPI(current, nproc, nx, xproc, dx);
  printf("rank=%d,size=%d,nproc=%d,xproc=%.4f\n", rank, size, nproc, xproc);

  // solve heat equation
  doubleArray ptrTemp;
  int temp = 0;
  for (int n = 0; n < nt; n++) {
    // setup leftSend, rightSend
    for (int j = 0; j < nx; j++) {
      leftSend[j] = current[0][j];
      rightSend[j] = current[nproc - 1][j];
    }
    // send left/right boundary information among processors
    MPI_Send(&leftSend, nx, MPI_DOUBLE, before, temp, MPI_COMM_WORLD);
    MPI_Recv(&right, nx, MPI_DOUBLE, after, temp, MPI_COMM_WORLD, &stats);

    MPI_Send(&rightSend, nx, MPI_DOUBLE, after, temp, MPI_COMM_WORLD);
    MPI_Recv(&left, nx, MPI_DOUBLE, before, temp, MPI_COMM_WORLD, &stats);

    // update temperature at current rank
    updateTemperatureMPI(current, next, k, nproc, nx, dx, dt, left, right);
    // switch old and new Temperature
    ptrTemp = current;
    current = next;
    next = ptrTemp;
  }

  // average Temperature
  double aveTemp = averageTemperatureMPI(next, nproc, nx);
  printf("Average temperature = %.4f\n", aveTemp);

  // write final temperature to files
  char fileName[50];
  sprintf(fileName, "heat_mpi_nx%d_nth%d_rank%d.dat", nx, size, rank);
  writeFileMPI(fileName, next, nproc, nx);

  // end MPI
  MPI_Finalize();

  // free memroy
  freeArrayMPI(current, nproc, nx);
  freeArrayMPI(next, nproc, nx);

  // time stop
  clock_t stop = clock();
  double elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
  printf("nx = %d, time [seconds] = %.4f\n", nx, elapsed);

  // end the program
  return 0;
}
