#ifndef TRANS_H
#define TRANS_H

#include "cachelab.h"
#include "stdio.h"

void transpose_submit(int M, int N, int A[N][M], int B[M][N]);

#endif