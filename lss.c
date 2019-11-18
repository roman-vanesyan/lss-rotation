#include "lss.h"

static
void PrintIntermediateRepresentation(int step, int size, double *A, double *B) {
    for (int i = 1; i < size + 1; i++) {
        printf("x%-14d", i);
    }

    printf("%-15c\n", 'B');

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%-15lf", A[i * size + j]);
        }

        printf("%-15lf\n", B[i]);
    }

    printf("\n");
}

int lss(int n, double *A, double *B, double *X, double *tmp) {
    double a1;
    double a2;
    double c;
    double s;
    double z;

    for (int i = 0; i < n; i++) {
        int step = i + 1;

        if (IsDebugFlagSet && step != n) {
            printf("--------------------\n");
            printf("Step: %d\n", step);
            printf("--------------------\n");
            printf("\n");
        }

        for (int j = i + 1; j < n; j++) {
            a1 = A[i * n + i];
            a2 = A[j * n + i];

            z = sqrt((a1 * a1) + (a2 * a2));

            if (z <= EPS) {
                if (IsDebugFlagSet) {
                    printf("Denominator of `c` and `s` equals to 0, skip step: %d\n", step);
                }

                continue;
            }

            c = a1 / z;
            s = a2 / z;

            for (int k = 0; k < n; k++) {
                a1 = A[i * n + k];
                a2 = A[j * n + k];

                A[i * n + k] = c * a1 + s * a2;
                A[j * n + k] = c * a2 - s * a1;

                if (fabs(A[i * n + k]) <= EPS) { A[i * n + k] = 0; }
                if (fabs(A[j * n + k]) <= EPS) { A[j * n + k] = 0; }
            }

            double b1 = B[i];
            double b2 = B[j];

            B[i] = c * b1 + s * b2;
            B[j] = c * b2 - s * b1;

            if (fabs(B[i]) <= EPS) { B[i] = 0; }
            if (fabs(B[j]) <= EPS) { B[j] = 0; }

            if (IsDebugFlagSet) {
                PrintIntermediateRepresentation(step, n, A, B);
            }
        }
    }

    if (IsDebugFlagSet) {
        printf("Forward elimination is ended!\n");
    }

    for (int i = n - 1; i >= 0; i--) {
        if (fabs(A[i * n + i]) <= EPS) {
            double sum = B[i];
            for (int j = i + 1; j < n; j++) { sum -= A[i * n + j] * X[j]; }

            if (fabs(sum) >= EPS) {
                if (IsDebugFlagSet) {
                    printf("Unresolvable system of linear equations, row %d!\n", i + 1);
                }

                return 1;
            }

            if (IsDebugFlagSet) {
                printf("Underdetermined system of linear equations, row: %d!\n", i + 1);
            }

            // доопределяем систему, задаем X.
            X[i] = 1;
        } else {
            X[i] = B[i] / A[i * n + i];
        }

        if (IsDebugFlagSet) {
            printf("X%d=%lf\n", i + 1, X[i]);
        }

        for (int j = 0; j < i; j++) {
            B[j] -= A[j * n + i] * X[i];

            if (fabs(B[j]) <= EPS) { B[j] = 0; }
        }
    }

    if (IsDebugFlagSet) {
        printf("Back substitution is ended!\n");
    }

    return 0;
}

int lss_memsize(int n) {
    return 0;
}
