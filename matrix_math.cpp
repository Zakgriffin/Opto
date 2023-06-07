#include "matrix_math.h"

#include <cstdio>

//struct Mat {
//    float *data;
//    int rows;
//    int cols;
//};
//

float *SquareMat::at(int x, int y) {
    return data + size * x + y;
}

void SquareMat::print() {
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            printf("%f ", data[r * size + c]);
        }
        printf("\n");
    }
}

//
//SquareMat mat_to_square_mat(Mat mat) {
//    if (mat.rows != mat.cols) {
//        printf("matrix is not square");
//        abort();
//    }
//    return {.data = mat.data, .size = mat.rows};
//}
//
//SquareMat get_cofactor(SquareMat mat, int p, int q) {
//    int i = 0;
//    int j = 0;
//
//    for (int row = 0; row < mat.size; row++) {
//        for (int col = 0; col < mat.size; col++) {
//            if (row != p && col != q) {
//                *sub_co_factors.at(i, j++) = *mat.at(row, col);
//
//                if (j == mat.size - 1) {
//                    j = 0;
//                    i++;
//                }
//            }
//        }
//    }
//}
//
//float determinant(SquareMat mat) {
//    float det = 0;
//
//    if (mat.size == 1) return *mat.at(0, 0);
//
//    float cofactors_data[mat.size][mat.size];
//    SquareMat cofactors = {
//            .data = (float *) &cofactors_data,
//            .size = mat.size
//    };
//
//    float sign = 1;
//    for (int f = 0; f < mat.size; f++) {
//        get_cofactor(mat, cofactors, 0, f);
//        det += sign * *mat.at(0, f) * determinant(cofactors);
//
//        sign = -sign;
//    }
//
//    return det;
//}
//
//void adjoint(SquareMat mat, SquareMat adj) {
//    if (mat.size == 1) {
//        *adj.at(0, 0) = 1;
//        return;
//    }
//
//    float sign;
//    int cofactors_data[mat.size][mat.size];
//    SquareMat cofactors = {
//            .data = (float *) &cofactors_data,
//            .size = mat.size
//    };
//
//    for (int i = 0; i < mat.size; i++) {
//        for (int j = 0; j < mat.size; j++) {
//            get_cofactor(mat, cofactors, i, j);
//
//            sign = ((i + j) % 2 == 0) ? 1 : -1;
//
//            *adj.at(j, i) = sign * (determinant(cofactors));
//        }
//    }
//}
//
//SquareMat inverse(SquareMat mat) {
//    float det = determinant(mat);
//    if (det == 0) {
//        printf("Singular matrix, can't find its inverse");
//        abort();
//    }
//
//    int adj_data[mat.size][mat.size];
//    SquareMat adj = {
//            .data = (float *) &adj_data,
//            .size = mat.size
//    };
//
//    adjoint(mat, adj);
//
//    auto *inverse_data = (float *) malloc(sizeof(float) * mat.size * mat.size);
//    SquareMat inverse = {
//            .data = inverse_data,
//            .size = mat.size
//    };
//    for (int i = 0; i < mat.size; i++) {
//        for (int j = 0; j < mat.size; j++) {
//            *inverse.at(i, j) = *adj.at(i, j) / det;
//        }
//    }
//
//    return inverse;
//}