#ifndef MANIPSURMATRICES_MATRIX3D_H
#define MANIPSURMATRICES_MATRIX3D_H
#include <stddef.h>

typedef struct {
    size_t depth;
    size_t rows;
    size_t cols;
    double *data;
} Matrix3D;

Matrix3D *mat3d_create(size_t depth, size_t rows, size_t cols);
void mat3d_free(Matrix3D *m);
double mat3d_get(const Matrix3D *m, size_t d, size_t i, size_t j);
void mat3d_set(Matrix3D *m, size_t d, size_t i, size_t j, double val);

Matrix3D *mat3d_mul(const Matrix3D *a, const Matrix3D *b);

void mat3d_print(const Matrix3D *m);

#endif //MANIPSURMATRICES_MATRIX3D_H