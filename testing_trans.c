#include <stdio.h>
#define BLOCK_SIZE_32 4

void trans_two_blocks(int M, int N, int c[N][M], int d[M][N], int block_size);
void transpose_submit(int M, int N, int A[N][M], int B[M][N]);
void print_matrix(int M, int N, int A[N][M]);
void init_matrix(int M, int N, int a[N][M]);

void print_matrix(int M, int N, int A[N][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("%i ", A[i][j]);
        }
        printf("\n");
    }
}
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int block_size;
    if ((M == 32 && N == 32) || (M == 64 && N == 64)) {
        if (M == 32 && N == 32)
            block_size = 8;
        else
            block_size = 4;

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                B[i][j] = A[i][j];

        for (int i = 0; i < N; i+=block_size) {
            int j = i;
            for (;j < M; j+=block_size)
                if (i == j)
                    trans_diag_block(M, N, &(B[i][j]), &(B[j][i]), block_size);
                else
                    trans_two_blocks(M, N, &(B[i][j]), &(B[j][i]), block_size);
        }
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

void trans_diag_block(int M, int N, int c[N][M], int d[M][N], int block_size) {
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

void init_matrix(int M, int N, int a[N][M]) {
    int counter = 10;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) {
            a[i][j] = counter;
            counter++;
        }

}

int main() {
    int n = 32, m = 32;
    int A[n][m];
    init_matrix(m, n, A);
    puts("before:");
    print_matrix(m, n, A);
    transpose_submit(m, n, A, A);
    puts("after: ");
    print_matrix(m, n, A);
    return 0;
}