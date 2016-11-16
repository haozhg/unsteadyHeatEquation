#include "writeFile.h"
#include <fstream>
#include <iostream>

int writerFile(string fileName, doubleArray T, int nx) {
  int i, j;
  ofstream output(fileName);
  if (output.is_open()) {
    for (j = 0; j < nx; j++) {
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
