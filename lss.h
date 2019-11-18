#ifndef LSS
#define LSS

#include <stdio.h>
#include <math.h>

#define EPS 1e-9

int IsErrorFlagSet;
int IsDebugFlagSet;

int lss(int n, double* A, double* B, double* X, double* tmp);
int lss_memsize(int n);

#endif
