// author: suzukaze

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

#define get(id, off) t##id = A[k][j + off]
#define set(id, off) B[j + off][k] = t##id

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

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
    int i, j, k, t0, t1, t2, t3, t4, t5, t6, t7;
    if (M == 32)
        for (i = 0; i < N; i += 8)
        {
            for (j = 0; j < M; j += 8)
            {
                for (k = i; k < i + 8; k++)
                {
                    get(0, 0), get(1, 1), get(2, 2), get(3, 3), get(4, 4), get(5, 5), get(6, 6), get(7, 7);
                    set(0, 0), set(1, 1), set(2, 2), set(3, 3), set(4, 4), set(5, 5), set(6, 6), set(7, 7);
                }
            }
        }
    else if (M == 64)
        for (i = 0; i < N; i += 4)
        {
            for (j = 0; j < M; j += 4)
            {
                for (k = i; k < i + 4; k++)
                {
                    get(0, 0), get(1, 1), get(2, 2), get(3, 3);
                    set(0, 0), set(1, 1), set(2, 2), set(3, 3);
                }
            }
        }
    else
        for (i = 0; i < N; i += 16)
        {
            for (j = 0; j < M; j += 16)
            {
                for (k = i; k < i + 16 && k < N; k++)
                {
                    get(0, 0), get(1, 1), get(2, 2), get(3, 3), get(4, 4), get(5, 5), get(6, 6), get(7, 7);
                    set(0, 0), set(1, 1), set(2, 2), set(3, 3), set(4, 4), set(5, 5), set(6, 6), set(7, 7);
                    if (M - j >= 16)
                    {
                        get(0, 8), get(1, 9), get(2, 10), get(3, 11), get(4, 12), get(5, 13), get(6, 14), get(7, 15);
                        set(0, 8), set(1, 9), set(2, 10), set(3, 11), set(4, 12), set(5, 13), set(6, 14), set(7, 15);
                    }
                    else
                    {
                        get(0, 8), get(1, 9), get(2, 10), get(3, 11), get(4, 12);
                        set(0, 8), set(1, 9), set(2, 10), set(3, 11), set(4, 12);
                    }
                }
            }
        }
    // 为啥按 8 分块要比 16 分块要差
    // for (i = 0; i < N; i += 8)
    // {
    //     for (j = 0; j < M; j += 8)
    //     {
    //         for (k = i; k < i + 8 && k < N; k++)
    //         {
    //             if (M - j >= 8)
    //             {
    //                 get(, 0), get(, 1), get(, 2), get(, 3), get(, 4), get(, 5), get(, 6), get(, 7);
    //                 set(, 0), set(, 1), set(, 2), set(, 3), set(, 4), set(, 5), set(, 6), set(, 7);
    //             }
    //             else
    //             {
    //                 get(, 0), get(, 1), get(, 2), get(, 3), get(, 4), get(, 5);
    //                 set(, 0), set(, 1), set(, 2), set(, 3), set(, 4), set(, 5);
    //             }
    //         }
    //     }
    // }
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

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
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

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}