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
                             const double x0proc, const double dx) {
  int i, j;
  double xi;
  // initial condition, boundary condition
  for (i = 0; i < nproc; i++) {
    xi = x0proc + i * dx;
    for (j = 0; j < nx; j++) {
      if (j == 0) {
        T[i][j] = cos(xi) * cos(xi);
      } else if (j == nx - 1) {
        T[i][j] = sin(xi) * sin(xi);
      } else {
        T[i][j] = 0;
      }
    }
  }
  return 0;
}

int updateTemperatureMPI(doubleArray current, doubleArray next, const double k,
                         const int nproc, const int nx, const double dx,
                         const double dt, const double *leftGhost,
                         const double *rightGhost) {
  int i, j;
  double laplaceT = 0;
  // update inner points [0,nx-1], [1,nx-2]
  for (i = 0; i < nproc; i++) {
    for (j = 1; j < nx - 1; j++) {
      if (i == 0) {
        // x=0
        laplaceT = (leftGhost[j] + current[i + 1][j] + current[i][j - 1] +
                    current[i][j + 1] - 4 * current[i][j]) /
                   (dx * dx);
      } else if (i == nproc - 1) {
        // x= pi
        laplaceT = (current[i - 1][j] + rightGhost[j] + current[i][j - 1] +
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
    for (j = 0; j < nx; j++) {
      for (i = 0; i < nproc; i++) {
        if (i < nproc - 1) {
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
  int numtasks, rank, rc; // total processes, current process rank
  int leftRank,
      rightRank; // leftGhost side (previous rank), rightGhost side (next rank)
  int nproc;     // domain decomposition (columns per process)
  double x0proc, averageT; // origin of current subdomain, average T current
  double leftGhost[nx], rightGhost[nx]; // leftGhost/rightGhost ghost boundary
                                        // of current subdomain
  double leftBoundary[nx], rightBoundary[nx]; // l/r boundary of the subdomain

  // MPI parallel
  MPI_Status Stat;
  rc = MPI_Init(&argc, &argv);
  if (rc != MPI_SUCCESS) {
    printf("Error starting MPI program. Terminating.\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
  }
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (nx % numtasks != 0) {
    printf("nx=%d can not be divided into numtasks=%d groups. Terminating.\n",
           nx, numtasks);
    MPI_Abort(MPI_COMM_WORLD, rc);
  }
  printf("rank =%d, MPI starting...\n", rank);
  // printf("Number of tasks= %d My rank= %d\n", numtasks, rank);

  // leftGhost side (previous rank), rightGhost side (next rank)
  leftRank = rank - 1;
  rightRank = rank + 1;
  if (rank == 0) {
    leftRank = numtasks - 1;
  }
  if (rank == numtasks - 1) {
    rightRank = 0;
  }

  // initialize subdomain temperature arrays, and allocate memory
  nproc = (int)nx / numtasks; // numtasks of subdomain (col)nproc*nx(row)
  doubleArray next = allocateArrayMPI(nproc, nx);
  doubleArray current = allocateArrayMPI(nproc, nx);

  // initialize Temperature
  x0proc = rank * PI / numtasks; // leftGhost origion of current subdomain
  initializeTemperatureMPI(next, nproc, nx, x0proc, dx);
  initializeTemperatureMPI(current, nproc, nx, x0proc, dx);
  printf("numtasks=%d,nproc=%d,rank=%d,x0proc=%.4f\n", numtasks, nproc, rank,
         x0proc);

  // solve heat equation
  doubleArray ptrTemp;
  int tag1 = 1, tag2 = 2;
  for (int n = 0; n < nt; n++) {
    // setup leftBoundary, rightBoundary
    for (int j = 0; j < nx; j++) {
      leftBoundary[j] = current[0][j];
      rightBoundary[j] = current[nproc - 1][j];
    }
    // send left boundary, and leftRank receive right ghost boundary
    MPI_Send(&leftBoundary, nx, MPI_DOUBLE, leftRank, tag1, MPI_COMM_WORLD);
    MPI_Recv(&rightGhost, nx, MPI_DOUBLE, rightRank, tag1, MPI_COMM_WORLD,
             &Stat);

    // send right boundary to rightRank, and rightRank receive left ghost
    // boundary
    MPI_Send(&rightBoundary, nx, MPI_DOUBLE, rightRank, tag2, MPI_COMM_WORLD);
    MPI_Recv(&leftGhost, nx, MPI_DOUBLE, leftRank, tag2, MPI_COMM_WORLD, &Stat);

    // update temperature at current rank
    updateTemperatureMPI(current, next, k, nproc, nx, dx, dt, leftGhost,
                         rightGhost);

    // switch old and new Temperature
    ptrTemp = current;
    current = next;
    next = ptrTemp;
  }

  // average Temperature
  double aveTemp = averageTemperatureMPI(next, nproc, nx);
  printf("current rank = %d, Average temperature = %.4f\n", rank, aveTemp);

  // write final temperature to files
  char fileName[50];
  sprintf(fileName, "heat_mpi_nx%d_nth%d_rank%d.dat", nx, numtasks, rank);
  writeFileMPI(fileName, next, nproc, nx);
  printf("Output file, heat_mpi_nx%d_nth%d_rank%d.dat\n", nx, numtasks, rank);

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
