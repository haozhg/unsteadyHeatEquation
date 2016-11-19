CXX = g++
CFLAGS = -g -Wall -std=c++0x
CFLAGSOMP = -g -Wall -std=c++0x -fopenmp

all: heat_serial heat_omp heat_mpi

heat_serial: heat_serial.cc
	$(CXX) $(CFLAGS) -o $@ $^

heat_omp: heat_omp.cc
	$(CXX) $(CFLAGSOMP) -o $@ $^

heat_mpi: heat_mpi.cc
	mpicc $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) .depend
	$(RM) *~
	$(RM) ./heat_serial
	$(RM) ./heat_omp
	$(RM) ./heat_mpi

depend:
	$(CXX) -MM $(CFLAGS) *.cc > .depend

-include .depend
