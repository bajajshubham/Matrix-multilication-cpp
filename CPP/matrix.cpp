#include <iostream>
#include <emscripten.h>

typedef int** Matrix;

int EMSCRIPTEN_KEEPALIVE getMatrixColumnsLength(const Matrix mat, int rows, int cols) {
    return cols;
}

int EMSCRIPTEN_KEEPALIVE getMatrixRowsLength(const Matrix mat, int rows, int cols) {
    return rows;
}

bool EMSCRIPTEN_KEEPALIVE areMatricesValidForMultiplication(const Matrix mat1, int rows1, int cols1, const Matrix mat2, int rows2, int cols2) {
    return cols1 == rows2;
}

Matrix EMSCRIPTEN_KEEPALIVE allocateMatrix(int rows, int cols) {
    Matrix mat = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        mat[i] = new int[cols];
    }
    return mat;
}

void EMSCRIPTEN_KEEPALIVE deallocateMatrix(Matrix mat, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] mat[i];
    }
    delete[] mat;
}

Matrix EMSCRIPTEN_KEEPALIVE multiplyMatrices(const Matrix mat1, int rows1, int cols1, const Matrix mat2, int rows2, int cols2) {
    if (areMatricesValidForMultiplication(mat1, rows1, cols1, mat2, rows2, cols2)) {
        Matrix result = allocateMatrix(rows1, cols2);
        for (int i = 0; i < rows1; ++i) {
            for (int j = 0; j < cols2; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < cols1; ++k) {
                    result[i][j] += mat1[i][k] * mat2[k][j];
                }
            }
        }
        return result;
    } else {
        return nullptr; // Multiplication not possible
    }
}

extern "C" {
    int* EMSCRIPTEN_KEEPALIVE temporaryMatrix(int rowLength, int columnLength) {
        return reinterpret_cast<int*>(allocateMatrix(rowLength, columnLength));
    }

    int EMSCRIPTEN_KEEPALIVE getMatrixColumnsLengthWrapper(int* mat, int rows, int cols) {
        return getMatrixColumnsLength(reinterpret_cast<Matrix>(mat), rows, cols);
    }

    int EMSCRIPTEN_KEEPALIVE getMatrixRowsLengthWrapper(int* mat, int rows, int cols) {
        return getMatrixRowsLength(reinterpret_cast<Matrix>(mat), rows, cols);
    }

    int EMSCRIPTEN_KEEPALIVE areMatricesValidForMultiplicationWrapper(int* mat1, int rows1, int cols1, int* mat2, int rows2, int cols2) {
        return areMatricesValidForMultiplication(reinterpret_cast<Matrix>(mat1), rows1, cols1, reinterpret_cast<Matrix>(mat2), rows2, cols2);
    }

    int* EMSCRIPTEN_KEEPALIVE multiplyMatricesWrapper(int* mat1, int rows1, int cols1, int* mat2, int rows2, int cols2) {
        return reinterpret_cast<int*>(multiplyMatrices(reinterpret_cast<Matrix>(mat1), rows1, cols1, reinterpret_cast<Matrix>(mat2), rows2, cols2));
    }

    void EMSCRIPTEN_KEEPALIVE deallocateMatrixWrapper(int* mat, int rows) {
        deallocateMatrix(reinterpret_cast<Matrix>(mat), rows);
    }
}
