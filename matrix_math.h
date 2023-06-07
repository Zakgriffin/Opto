#ifndef OPTO_MATRIX_MATH_H
#define OPTO_MATRIX_MATH_H

struct SquareMat {
    float *data;
    int size;

    float *at(int x, int y);

    void print();
};
#endif //OPTO_MATRIX_MATH_H
