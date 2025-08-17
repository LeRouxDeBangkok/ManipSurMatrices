#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

/*
xmalloc = gestion dechec dallocation
index_of calcule l'index 1D de (i,j)
scanf("%lf", ...) lit les doubles en C, avec un point pas une virgule.
mat_print formate une ligne par ligne
Condition dalignement: A.cols == B.rows
*/

static size_t index_of(size_t i, size_t j, size_t cols) {
    return i * cols + j; //position (i,j) dans data
}

static void *xmalloc(size_t n) {
    void *p = malloc(n);
    if (!p) {
        fprintf(stderr, "memoire insuffisante\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

Matrix *mat_create(size_t rows, size_t cols) {
    Matrix *m = (Matrix *)xmalloc(sizeof(double) *rows *cols);
    m->rows = rows;
    m->cols = cols;
    m->data = (double *)xmalloc(sizeof(double) * rows * cols);
    //initialisation a 0
    for (size_t k =0; k < rows * cols; k++) m->data[k] = 0.0;
    return m;
}

void mat_free(Matrix *m) {
    if (m) {
        free(m->data);
        free(m);
    }
}

double mat_get(const Matrix *m, size_t i, size_t j) {
    return m->data[index_of(i, j, m->cols)];
}

void mat_set(Matrix *m, size_t i, size_t j, double value) {
    m->data[index_of(i, j, m->cols)] = value;
}

int mat_read(Matrix *m) {
    for (size_t i =0; i< m->rows; ++i) {
        for (size_t j =0; j< m->cols; ++j) {
            double x;
            if (scanf("%lf", &x) != 1) {
                return 0; //erreur de lecture
            }
            mat_set(m, i, j, x);
        }
    }
    return 1;
}

void mat_print(const Matrix *m) {
    for (size_t i =0; i < m->rows; ++i) {
        for (size_t j = 0; j < m->cols; ++j) {
            printf("%g", mat_get(m, i, j));
            if (j + 1 < m->cols) printf(" ");
        }
        printf("\n");
    }
}

Matrix *mat_mul(const Matrix *A, const Matrix *B) {
    if (A->cols != B->rows) return NULL; //impossible de multiplier

    Matrix *C = mat_create(A->rows, B->cols);

    for (size_t i =0; i < A->rows; ++i) {
        for (size_t k = 0; k < A->cols; ++k) {
            double aik = mat_get(A, i, k);
            for (size_t j = 0; j < B->cols; ++j) {
                C->data[index_of(i, j, C->cols)] += aik * B->data[index_of(k, j, B->cols)];
            }
        }
    }
    return C;
}