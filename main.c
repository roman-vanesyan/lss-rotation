#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lss.h"

int GetStringLength(const char *string) {
    int i = 0;

    for (; string[i] != 0; i++);

    return i;
}

int CompareStrings(const char *left, const char *right) {
    int lsize = GetStringLength(left);
    int rsize = GetStringLength(right);

    if (lsize != rsize) {
        return 0;
    }

    for (int i = 0; i < lsize; i++) {
        if (left[i] != right[i]) {
            return 0;
        }
    }

    return 1;
}

int IsErrorCode(int code) {
    return code != 0;
}

int ReadMatrix(FILE *in, double **A, int size) {
    if (in == NULL) {
        if (IsErrorFlagSet) {
            printf("ERR: error argument, in file is NULL!\n");
        }

        return 100;
    }

    *A = (double *) malloc(size * size * sizeof(double));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (fscanf(in, "%lf", &(*A)[i * size + j]) != 1) {
                return 101;
            }
        }
    }

    return 0;
}

int ReadVector(FILE *in, double **B, int size) {
    if (in == NULL) {
        if (IsErrorFlagSet) {
            printf("ERR: error argument, in file is NULL!\n");
        }

        return 100;
    }

    *B = (double *) malloc(size * size * sizeof(double));

    for (int i = 0; i < size; i++) {
        if (fscanf(in, "%lf", &(*B)[i]) != 1) {
            return 101;
        }
    }

    return 0;
}

int ReadInput(const char *input, double **A, double **B, int *size) {
    FILE *in = fopen(input, "r");
    int code = 0;

    if (in == NULL) {
        if (IsErrorFlagSet) {
            printf("ERR: Unable to open %s in read mode!\n", input);
        }

        return 100;
    }

    if (fscanf(in, "%d", &(*size)) != 1) {
        code = 101;

        goto LEND;
    }

    code = ReadMatrix(in, A, *size);

    if (IsErrorCode(code)) {
        goto LEND;
    }

    code = ReadVector(in, B, *size);

    if (IsErrorCode(code)) {
        goto LEND;
    }

    LEND:
    if (IsErrorFlagSet && IsErrorCode(code)) {
        printf("ERR: Too few data provided while reading %s!\n", input);
    }

    fclose(in);

    return code;
}

int WriteOutput(const char *output, int isResolved, int size, double *X) {
    int code = 0;

    FILE *out = fopen(output, "w");
    if (out == NULL) {
        if (IsErrorFlagSet) {
            printf("ERR: Unable to open %s in write mode!\n", output);
        }

        code = 100;

        goto RET;
    }

    if (isResolved) {
    if (fprintf(out, "%d\n", size) <= 0) {
        code = 102;

        if (IsErrorFlagSet) {
            printf("ERR: Unable to write to file %s!\n", output);
        }

        goto LEND;
    }

    for (int i = 0; i < size; i++) {
        if (fprintf(out, "%1.9lf\n", X[i]) <= 0) {
            code = 102;

            if (IsErrorFlagSet) {
                printf("ERR: Unable to write to file %s!\n", output);
            }

            break;
        }
    }
    } else {
        if (fprintf(out, "%d\n", 0) <= 0) {
            code = 102;

            if (IsErrorFlagSet) {
                printf("ERR: Unable to write to file %s!\n", output);
            }

            goto LEND;
        }
    }

    LEND:
    fclose(out);

    RET:
    return code;
}

void PrintMatrix(int size, double *A) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%-15lf", A[i * size + j]);
        }

        printf("\n");
    }

    printf("\n");
}

void PrintHelpMessage() {
    printf("./lss$ ./lss -?\n");
    printf(" Usage: lss [input_file_name] [output_file_name] [options]\n");
    printf("Where options include:\n");
    printf("-d        print debug messages [default OFF]\n");
    printf("-e        print errors [default OFF]\n");
    printf("-p        print matrix [default OFF]\n");
    printf("-t        print execution time [default OFF]\n");
    printf("-h, -?    print this and exit\n");
    printf("Default input_file_name value is lss_in.txt, default output_file_name value is lss_out.txt.\n");
}

int ReadCommandLineArguments(
        int count,
        char *arguments[],
        int *isHelpFlagSet,
        int *isShowMatrixFlagSet,
        int *isTraceFlagSet,
        char **input,
        char **output
) {
    int code = 0;
    int isInputSet = 0;
    int isOutputSet = 0;

    for (int i = 1; i < count; i++) {
        char *arg = arguments[i];

        if (CompareStrings(arg, "-h") || CompareStrings(arg, "-?")) {
            *isHelpFlagSet = 1;
        } else if (CompareStrings(arg, "-d")) {
            IsDebugFlagSet = 1;
        } else if (CompareStrings(arg, "-e")) {
            IsErrorFlagSet = 1;
        } else if (CompareStrings(arg, "-t")) {
            *isTraceFlagSet = 1;
        } else if (CompareStrings(arg, "-p")) {
            *isShowMatrixFlagSet = 1;
        } else if (arg[0] == '-') {
            code = 202;

            goto RET;
        } else {
            if (*isHelpFlagSet ||
                IsDebugFlagSet ||
                IsErrorFlagSet ||
                *isTraceFlagSet ||
                *isShowMatrixFlagSet) {
                code = 200;

                goto RET;
            } else if (!isInputSet) {
                *input = arg;
                isInputSet = 1;
            } else if (!isOutputSet) {
                *output = arg;
                isOutputSet = 1;
            } else {
                code = 201;

                goto RET;
            }
        }
    }

    RET:
    return code;
}

int main(int argc, const char *argv[]) {
    int code = 0;

    char *input = "lss_in.txt";
    char *output = "lss_out.txt";

    int isHelpFlagSet = 0;
    int isShowMatrixFlagSet = 0;
    int isTraceFlagSet = 0;
    IsErrorFlagSet = 0;
    IsDebugFlagSet = 0;

    code = ReadCommandLineArguments(
            argc,
            (char **) argv,
            &isHelpFlagSet,
            &isShowMatrixFlagSet,
            &isTraceFlagSet,
            &input,
            &output
    );

    if (IsErrorCode(code)) {
        goto RET;
    }

    if (isHelpFlagSet) {
        PrintHelpMessage();

        goto RET;
    }

    int size;
    double *A = NULL;
    double *X = NULL;
    double *B = NULL;
    double *tmp = NULL;

    code = ReadInput(input, &A, &B, &size);

    if (IsErrorCode(code)) {
        goto LEND;
    }

    if (isShowMatrixFlagSet) {
        PrintMatrix(size, A);
    }

    X = (double *) malloc(size * sizeof(double));
    B = (double *) malloc(size * sizeof(double));
    tmp = (double *) malloc(lss_memsize(size) * sizeof(double));

    time_t start = clock();
    code = lss(size, A, B, X, tmp);

    if (IsErrorCode(code)) {
        if (IsErrorFlagSet) {
            printf("ERR: Unable to find the solution of the linear system of equations (possibly unresolvable system of linear equations)!\n");
        }
    }

    double duration = (double) (clock() - start) / CLOCKS_PER_SEC;

    if (isTraceFlagSet) {
        printf("TRACE: the result was computed in %lfsec.\n", duration);
    }

    int writeErrCode = WriteOutput(output, code == 0, size, X);

    if (IsErrorCode(writeErrCode)) {
        code = writeErrCode;
    }

    LEND:
    free(A);
    free(B);
    free(X);
    free(tmp);

    A = NULL;
    B = NULL;
    X = NULL;
    tmp = NULL;

    RET:
    return code;
}
