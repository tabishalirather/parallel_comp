// For output to the terminal
#include <stdio.h>
#include <omp.h>

// For the random number commands
#include <stdlib.h>

// For the square root function
#include <math.h>

/*
 * Write a matrix into the terminal
 */
void printmatrix(double **a, int rows, int columns)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            printf("%f ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
 * Write a vector into the terminal
 */
void printvector(double *b, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%f \n", b[i]);
    }
    printf("\n");
}

/*
 * Need to set rows = 4 and columns = 4 for this to work
 */
void small_example(double **a, double *x)
{
    // row 0
    a[0][0] = 1.0;
    a[0][1] = 2.0;
    a[0][2] = 3.0;
    a[0][3] = 0.0;

    // row 1
    a[1][0] = 6.0;
    a[1][1] = 0.0;
    a[1][2] = 1.0;
    a[1][3] = 2.0;

    // row 2
    a[2][0] = 1.0;
    a[2][1] = 1.0;
    a[2][2] = 0.0;
    a[2][3] = 0.0;

    // row 3
    a[3][0] = 2.0;
    a[3][1] = 0.0;
    a[3][2] = 0.0;
    a[3][3] = 0.0;

    // vector
    x[0] = 1.0;
    x[1] = 1.0;
    x[2] = 1.0;
    x[3] = 1.0;
}

/*
 * Example where all entries of A and x are 1. The entries in b are all equal to the number of columns in this case.
 */
void large_example(double **a, double *x, int rows, int columns)
{

    for (int j = 0; j < columns; j++)
    {
        for (int i = 0; i < rows; i++)
        {
            a[i][j] = 1.0;
        }

        x[j] = 1.0;
    }
}

int main(int argc, char **argv)
{
    int rows = 4;
    int columns = 4;

    // int rows = 1048;
    // int columns = 1024;

    int i, j;
    double sum;
    // allocate memory for matrix A and vectors x and b
    double *x = (double *)malloc(columns * sizeof(double));
    double *b = (double *)malloc(rows * sizeof(double));
    double **a = (double **)malloc(rows * sizeof(double *));
    for (i = 0; i < rows; i++)
    {
        a[i] = (double *)malloc(columns * sizeof(double));
    }

    if (rows == 4)
    {
        /*
         * Small example should result in
         *  b[0] =  6.0
         *  b[1] =  9.0
         *  b[2] =  2.0
         *  b[3] =  2.0
         */
        small_example(a, x);
    }
    else
    {
        /*
         * All entries in b should be equal to the number of columns
         */
        large_example(a, x, rows, columns);
    }

    // print matrix and vector only for the small example
    if (rows == 4)
    {
        puts("This is the matrix in rows==4");
        printmatrix(a, rows, columns);
        puts("This is the vector in rows==4");
        printvector(x, columns);
    }

    /*
     * This is the main part, doing the actual computation
     */
    for (i = 0; i < rows; i++)
    {

        sum = 0.0;
        for (j = 0; j < columns; j++)
        {
            sum += a[i][j] * x[j];
        }
        b[i] = sum;
    }

    // output the result
    if (rows == 4)
    {
        puts("This is the result vector");
        printvector(b, rows);
    }
    else
    {
        printf("First element of b: %i \n", (int)b[0]);
        printf("Last element of b: %i \n", (int)b[rows - 1]);
    }
    free(b);


    //Now we want to parallelize this.
    double* b_prll = (double *)malloc(rows * sizeof(double));
    omp_set_num_threads(4);
// #pragma omp parallel
//     {
//
//         printf("Hello World... from thread = %d\n",
//                omp_get_thread_num());
//     }
    puts("this is the start of parallel");
    #pragma omp
    #pragma omp parallel
    {
        printf("Hello World... from thread = %d\n", omp_get_thread_num());
        for (int i = 0; i < 3; i++)
        {
            printf("i:%d\n", i);
        }

    }
    puts("this is end of parallel");

    // printvector(b_prll, rows);






    free(a);
    // free(b);
    free(b_prll);
    free(x);
}
