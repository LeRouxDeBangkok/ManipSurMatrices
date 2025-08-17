#ifndef MANIPSURMATRICES_MATRIX_H
#define MANIPSURMATRICES_MATRIX_H
#include <stddef.h> //size_t

//definition du type matrix et les fonctions publiques

typedef struct {
    size_t rows;  //nombre de lignes
    size_t cols;  //nombre de colonnes
    double *data; //tableau contigu de taille rows*cols
} Matrix;

// Creation/destruction
Matrix *mat_create(size_t rows, size_t cols);
void mat_free(Matrix *m);

//Acces elementaire
double mat_get(const Matrix *m, size_t i, size_t j);
void mat_set(Matrix *m, size_t i, size_t j, double value);

//I/O texte
int mat_read(Matrix *m); //lis rows*cols nombres depuis stdin; 1=ok 0=erreur
void mat_print(const Matrix *m); //affiche en lignes, separes par espaces

//Operation
Matrix *mat_mul(const Matrix *A, const Matrix *B); //NULL si dimensions incompatibles

#endif //MANIPSURMATRICES_MATRIX_H