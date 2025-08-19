#include "matrix.h"
#include "matrix3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ---- utilitaires ----
static size_t read_size(const char *prompt) {
    long long v;
    for (;;) {
        printf("%s", prompt);
        if (scanf("%lld", &v) == 1 && v > 0) return (size_t)v;
        fprintf(stderr, "Entrez un entier positif.\n");
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
    }
}

static int menu(void) {
    int choix;
    printf("\n=== MENU MATRICES ===\n");
    printf("1. Multiplication de deux matrices 2D\n");
    printf("2. Rotation de matrice\n");
    printf("3. Inversion de matrice\n");
    printf("4. Multiplication de deux matrices 3D\n");
    printf("0. Quitter\n");
    printf("Votre choix: ");
    if (scanf("%d", &choix) != 1) {
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
        return -1;
    }
    return choix;
}

// ---- opérations ----
static void do_matrix_multiplication(void) {
    size_t a_rows = read_size("A lignes: ");
    size_t a_cols = read_size("A colonnes: ");
    size_t b_rows = read_size("B lignes: ");
    size_t b_cols = read_size("B colonnes: ");

    if (a_cols != b_rows) {
        fprintf(stderr, "Dimensions incompatibles: %zu != %zu\n", a_cols, b_rows);
        return;
    }

    Matrix *A = mat_create(a_rows, a_cols);
    Matrix *B = mat_create(b_rows, b_cols);

    printf("Entrez les %zu x %zu valeurs de A:\n", a_rows, a_cols);
    if (!mat_read(A)) {
        fprintf(stderr, "Erreur de lecture A\n");
        mat_free(A); mat_free(B);
        return;
    }

    printf("Entrez les %zu x %zu valeurs de B:\n", b_rows, b_cols);
    if (!mat_read(B)) {
        fprintf(stderr, "Erreur de lecture B\n");
        mat_free(A); mat_free(B);
        return;
    }

    Matrix *C = mat_mul(A, B);
    if (!C) {
        fprintf(stderr, "Erreur de multiplication\n");
        mat_free(A); mat_free(B);
        return;
    }

    printf("La matrice C de dimensions (%zu x %zu):\n", C->rows, C->cols);
    mat_print(C);

    mat_free(A);
    mat_free(B);
    mat_free(C);
}

static void do_matrix_multidim(void) {
    size_t depth = read_size("Profondeur (nb de matrices 2D): ");
    size_t a_rows = read_size("A lignes: ");
    size_t a_cols = read_size("A colonnes: ");
    size_t b_rows = read_size("B lignes: ");
    size_t b_cols = read_size("B colonnes: ");

    if (a_cols != b_rows) {
        fprintf(stderr, "Dimensions incompatibles\n");
        return;
    }

    Matrix3D *A = mat3d_create(depth, a_rows, a_cols);
    Matrix3D *B = mat3d_create(depth, b_rows, b_cols);

    for (size_t k = 0; k < depth; k++) {
        printf("Tranche %zu de A (%zu x %zu):\n", k, a_rows, a_cols);
        for (size_t i =0; i < a_rows; i++) {
            for (size_t j = 0; j < a_cols; j++) {
                double v;
                if (scanf("%lf", &v) != 1) {
                    fprintf(stderr, "Erreur lecture\n");
                    mat3d_free(A);
                    mat3d_free(B);
                    return;
                }
                mat3d_set(A, k, i, j, v);
            }
        }
    }
    for (size_t k = 0; k < depth; k++) {
        printf("Tranche %zu de B (%zu x %zu):\n", k, b_rows, b_cols);
        for (size_t i = 0; i < b_rows; i++) {
            for (size_t j = 0; j < b_cols; j++) {
                double v;
                if (scanf("%lf", &v) != 1) {
                    fprintf(stderr, "Erreur lecture\n");
                    mat3d_free(A);
                    mat3d_free(B);
                    return;
                }
                mat3d_set(B, k, i, j, v);
            }
        }
    }

    Matrix3D *C = mat3d_mul(A, B);
    if (!C) {
        fprintf(stderr, "Erreur de multiplication\n");
        mat3d_free(A);
        mat3d_free(B);
        return;
    }

    for (size_t k = 0; k < depth; k++) {
        printf("Tranche %zu de C (%zu x %zu):\n", k, C->rows, C->cols);
        for (size_t i = 0; i < C->rows; i++) {
            for (size_t j = 0; j < C->cols; j++) {
                printf("%8.3f ", mat3d_get(C, k, i, j));
            }
            printf("\n");
        }
    }

    mat3d_free(A);
    mat3d_free(B);
    mat3d_free(C);
}

static void do_matrix_inversion(void) {
    size_t n = read_size("Taille de la matrice carree A (n): ");

    Matrix *A = mat_create(n, n);
    printf("Entrez les %zu x %zu valeurs de la matrice A:\n", n, n);
    if (!mat_read(A)) {
        fprintf(stderr, "Erreur de lecture\n");
        mat_free(A);
        return;
    }

    Matrix *aug = mat_create(n, 2*n);

    for (size_t i =0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            mat_set(aug, i, j, mat_get(A, i, j));
        }
        for (size_t j = 0; j < n; j++) {
            mat_set(aug, i, n+j, (i == j) ? 1.0 : 0.0);
        }
    }

    for (size_t k = 0; k < n; k++) {
        size_t pivot_row = k;
        double max_val = fabs(mat_get(aug, k, k));
        for (size_t i = k + 1; i < n; i++) {
            double val = fabs(mat_get(aug, i, k));
            if (val > max_val) {
                max_val = val;
                pivot_row = i;
            }
        }
        if (max_val == 0.0) {
            fprintf(stderr, "Matrice non inversible\n");
            mat_free(A);
            mat_free(aug);
            return;
        }
        if (pivot_row != k) {
            for (size_t j = 0; j < 2*n; j++) {
                double tmp = mat_get(aug, k, j);
                mat_set(aug, k, j, mat_get(aug, pivot_row, j));
                mat_set(aug, pivot_row, j, tmp);
            }
        }
        double pivot = mat_get(aug, k, k);
        for (size_t j =0; j<2*n; j++) {
            double val = mat_get(aug, k, j) / pivot;
            mat_set(aug, k, j, val);
        }
        for (size_t i = 0; i < n; i++) {
            if (i == k) continue;
            double factor = mat_get(aug, i, k);
            for (size_t j =0; j < 2*n; j++) {
                double val = mat_get(aug, i, j) - factor * mat_get(aug, k, j);
                mat_set(aug, i, j, val);
            }
        }
    }

    Matrix *inv = mat_create(n, n);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            mat_set(inv, i, j, mat_get(aug, i, n+j));
        }
    }

    printf("Inverse de A (%zu x %zu)\n", n, n);
    mat_print(inv);

    mat_free(A);
    mat_free(aug);
    mat_free(inv);
}


static void do_matrix_rotation(void) {
    size_t n = read_size("Taille de la matrice carree A (n): ");
    Matrix *A = mat_create(n, n);
    printf("Entrez les %zu x %zu valeurs de la matrice A:\n", n, n);
    if (!mat_read(A)) {
        fprintf(stderr, "Erreur de lecture\n");
        mat_free(A);
        return;
    }

    int angle;
    printf("Angle de rotation (90, 180, 270): ");
    if (scanf("%d", &angle) != 1) {
        fprintf(stderr, "Lecture angle echouee\n");
        mat_free(A);
        return;
    }

    Matrix *R = mat_create(n, n);
    if (angle == 90) {
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                mat_set(R, i, j, mat_get(A, n - j - 1, i));
            }
        }
    } else if (angle == 180) {
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                mat_set(R, i, j, mat_get(A, n - i - 1, n - j - 1));
            }
        }
    } else if (angle == 270) {
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                mat_set(R, i, j, mat_get(A, j, n - i - 1));
            }
        }
    } else {
        fprintf(stderr, "Angle non pris en charge\n");
        mat_free(A);
        mat_free(R);
        return;
    }

    printf("Matrice A tournee de %d degrees\n", angle);
    mat_print(R);
    mat_free(A);
    mat_free(R);
}

// ---- point d'entrée ----
int main(void) {
    for (;;) {
        int choix = menu();
        if (choix == 0) {
            printf("Au revoir.\n");
            break;
        }
        else if (choix == 1) {
            do_matrix_multiplication();
        }
        else if (choix == 2) {
            do_matrix_rotation();
        }
        else if (choix == 3) {
            do_matrix_inversion();
        }
        else if (choix == 4) {
            do_matrix_multidim();
        }
        else {
            printf("Choix invalide.\n");
        }
    }
    return EXIT_SUCCESS;
}
