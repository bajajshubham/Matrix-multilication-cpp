#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>

// mat1[R1][C1] and mat2[R2][C2]
#define R1 2 // number of rows in Matrix-1
#define C1 2 // number of columns in Matrix-1
#define R2 2 // number of rows in Matrix-2
#define C2 2 // number of columns in Matrix-2

int* EMSCRIPTEN_KEEPALIVE multiplyMatrices(int mat1[R1][C1], int mat2[R2][C2]) {
  // Dynamically allocate memory for the result matrix
  int* rslt = (int*)malloc(R1 * C2 * sizeof(int));

  for (int i = 0; i < R1; i++) {
    for (int j = 0; j < C2; j++) {
      rslt[i * C2 + j] = 0;

      for (int k = 0; k < R2; k++) {
        rslt[i * C2 + j] += mat1[i][k] * mat2[k][j];
      }
    }
  }

  // Return a pointer to the first element of the result matrix
  return rslt;
}
