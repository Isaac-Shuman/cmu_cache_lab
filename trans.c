/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"
#define BLOCK_SIZE_32 8

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void trans_two_blocks(int M, int N, int c[N][M], int d[M][N], int block_size);
void trans_diag_block(int M, int N, int c[N][M], int block_size);
void trans(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int block_size;
    if ((M == 32 && N == 32) || (M == 64 && N == 64)) {
        if (M == 32 && N == 32)
            block_size = 8;
        else
            block_size = 4;
    
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++) //hits:1, misses:2052, evictions:2020
                B[i][j] = A[i][j];

        // for (int i = 0; i < N; i+=block_size) {
        //     int j = i;
        //     for (;j < M; j+=block_size)
        //         if (i == j)
        //             trans_diag_block(M, N, (int (*)[M]) &(B[i][j]), block_size);
        //         else
        //             trans_two_blocks(M, N, (int (*)[M]) &(B[i][j]), (int (*)[N]) &(B[j][i]), block_size);
        // }
    }
}

void trans_two_blocks(int M, int N, int c[N][M], int d[M][N], int block_size) {
    int tmp;
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            tmp = c[i][j]; // tmp = c[0][1]
            c[i][j] = d[j][i]; // c[0][1] = d[1][0]
            d[j][i] = tmp; //d[1][0]
        }
    }
}

void trans_diag_block(int M, int N, int c[N][M], int block_size) {
    int tmp;
    for (int i = 0; i < block_size; i++) {
        int j = i;
        for (; j < block_size; j++) {
            tmp = c[i][j]; // tmp = c[0][1]
            printf("this iteration tmp is %i\n", tmp);
            c[i][j] = c[j][i]; // c[0][1] = d[1][0]
            c[j][i] = tmp; //d[1][0]
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

