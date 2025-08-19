#include "matrix3d.h"
#include <stdio.h>
#include <stdlib.h>

Matrix3D *mat3d_create(size_t depth, size_t rows, size_t cols) {
    Matrix3D *m = malloc(sizeof(Matrix3D));
    if (!m) return NULL;
    m->depth = depth;
    m->rows = rows;
    m->cols = cols;
    m->data = calloc(depth * rows * cols, sizeof(double));
    if (!m->data) {
        free(m);
        return NULL;
    }
    return m;
}

void mat3d_free(Matrix3D *m) {
    if (m) {
        free(m->data);
        free(m);
    }
}

double mat3d_get(const Matrix3D *m, size_t d, size_t i, size_t j) {
    return m->data[d * (m->rows * m->cols) + i * m->cols + j];
}

void mat3d_set(Matrix3D *m, size_t d, size_t i, size_t j, double val) {
    m->data[d * (m->rows * m->cols) + i * m->cols + j] = val;
}

Matrix3D *mat3d_mul(const Matrix3D *a, const Matrix3D *b) {
    if (!a || !b) return NULL;
    if (a->depth != b->depth) return NULL;
    if (a->cols != b->rows) return NULL;

    Matrix3D *c = mat3d_create(a->depth, a->rows, a->cols);
    if (!c) return NULL;

    for (size_t d = 0; d < a->depth; d++) {
        for (size_t i = 0; i < a->rows; i++) {
            for (size_t j = 0; j < b->cols; j++) {
                double sum = 0.0;
                for (size_t k = 0; k < a->cols; k++) {
                    sum += mat3d_get(a, d, i, k) * mat3d_get(b, d, k, j);
                }
                mat3d_set(c, d, i, j, sum);
            }
        }
    }
    return c;
}

void mat3d_print(const Matrix3D *m) {
    if (!m) {
        printf("(null)\n");
        return;
    }
    for (size_t d = 0; d < m->depth; d++) {
        printf("Tranche %zu:\n", d);
        for (size_t i = 0; i < m->rows; i++) {
            for (size_t j = 0; j < m->cols; j++) {
                printf("%8.3f ", mat3d_get(m, d, i, j));
            }
            printf("\n");
        }
        printf("\n");
    }
}