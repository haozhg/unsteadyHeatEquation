all: heat_serial heat_omp

heat_serial : heat_serial.cc updateTserial.cc doubleArray.cc initializeT.cc writeFile.cc averageT.cc
	g++ -std=c++0x -o heat_serial heat_serial.cc updateTserial.cc doubleArray.cc initializeT.cc writeFile.cc averageT.cc

heat_omp : heat_omp.cc updateTomp.cc doubleArray.cc initializeT.cc writeFile.cc averageT.cc
	g++ -std=c++0x -fopenmp -o heat_omp heat_omp.cc updateTomp.cc doubleArray.cc initializeT.cc writeFile.cc averageT.cc
