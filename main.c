#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

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
    printf("2. Multiplication de deux matrices multidimensionelles (a venir)\n");
    printf("3. Inversion de matrice (a venir)\n");
    printf("4. Rotation de matrice (a venir)\n");
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
    printf("WIP\n");
}

static void do_matrix_inversion(void) {
    printf("WIP\n");
}

static void do_matrix_rotation(void) {
    printf("WIP\n");
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
            do_matrix_multidim();
        }
        else if (choix == 3) {
            do_matrix_inversion();
        }
        else if (choix == 4) {
            do_matrix_rotation();
        }
        else {
            printf("Choix invalide.\n");
        }
    }
    return EXIT_SUCCESS;
}
