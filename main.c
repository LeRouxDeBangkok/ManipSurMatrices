#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

static size_t read_size(const char *prompt) {
    long long v;
    for (;;) {
        printf("%s", prompt);
        if (scanf("%lf", &v) == 1 && v > 0) return (size_t)v;
        fprintf(stderr, "Entrez un entier positif.\n");
        int c; while ((c = getchar()) != '\n' && c != EOF); {/* vider */}
    }
}

int main (void) {
    printf("Multiplications de matrices A(lignes x colonnes) et B(lignes x colonnes)\n");

    size_t a_rows = read_size("A lignes: ");
    size_t a_cols = read_size("A colonnes: ");
    size_t b_rows = read_size("B lignes: ");
    size_t b_cols = read_size("B colonnes: ");

    if (a_cols != b_rows) {
        fprintf(stderr, "Les dimensions sont incompatibles");
        return EXIT_FAILURE;
    }

    Matrix *A = mat_create(a_rows, a_cols);
    Matrix *B = mat_create(b_rows, b_cols);

    printf("Entrez les %zu x %zu valeurs de A, ligne par ligne:\n", a_rows, a_cols);
    if (!mat_read(A)) {
        fprintf(stderr, "Erreur dans la lecture de A\n");
        mat_free(A);
        mat_free(B);
        return EXIT_FAILURE;
    }
    printf("Entrez les %zu x %zu valeurs de B ligne par ligne:\n", b_rows, b_cols);
    if (!mat_read(B)) {
        fprintf(stderr, "Erreur dans la lecture de B\n");
        mat_free(A);
        mat_free(B);
        return EXIT_FAILURE;
    }

    Matrix *C = mat_mul(A, B);
    if (!C) {
        fprintf(stderr, "Erreur de multiplication\n");
        mat_free(A);
        mat_free(B);
        return EXIT_FAILURE;
    }

    printf("La matrice C = A * B de dimensions (%zu x &zu):\n", C->rows, C->cols);
    mat_print(C);

    mat_free(A);
    mat_free(B);
    mat_free(C);
    return EXIT_SUCCESS;
}