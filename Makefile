all: heat_serial heat_omp heat_mpi

heat_serial : heat_serial.cc
	g++ -std=c++0x -g -Wall -o heat_serial heat_serial.cc

heat_omp : heat_omp.cc
	g++ -std=c++0x -fopenmp -g -Wall -o heat_omp heat_omp.cc

heat_mpi : heat_mpi.cc
	mpicc -std=c++0x -g -Wall -o heat_mpi heat_mpi.cc
