// For output to the terminal

#include <stdio.h>

// For the random number commands
#include <stdlib.h>

// For the square root function
#include <math.h>

/*
 * Write a matrix into the terminal
 */
void printmatrix(double** a, int rows, int columns)
{
    int i, j;
    
    for (i=0; i<rows; i++)
    {
     for (j=0; j<columns; j++)
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
void printvector(double* b, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
     printf("%f \n", b[i]);   
    }
    printf("\n");
}

/*
 * Need to set rows = 3 and columns = 4 for this to work
 */
void small_example(double** a, double* x)
{
    
    // column 0
    a[0][0] = 1.0;
    a[1][0] = 1.0;
    a[2][0] = 0.0;
    
    // column 1
    a[0][1] = -1.0;
    a[1][1] = 1.0;
    a[2][1] = 1.0;
    
    // column 2
    a[0][2] = 1.0;
    a[1][2] = 1.0;
    a[2][2] = 1.0;
    
    // column 3
    a[0][3] = 0.0;
    a[1][3] = 0.0;
    a[2][3] = 0.0;
    
    x[0] = -0.5;
    x[1] = 0.5;
    x[2] = 1.0;
    x[3] = 13.0;    
}

/*
 * Example where all entries of A and x are 1. The entries in b are all equal to the number of columns in this case.
 */
void large_example(double** a, double* x, int rows, int columns)
{

 for (int j=0; j<columns; j++)
 {
      for (int i=0; i<rows; i++)
      {
            a[i][j] = 1.0;
      }      
      
      x[j] = 1.0;

 }
 
 
}

int main(int argc, char** argv)
{
    // int rows = 3;
    // int columns = 4;
    
    int rows = 4531;
    int columns = 9427;
    
    int i, j;
    double sum;
    // allocate memory for matrix A and vectors x and c
    double *x = (double *) malloc(columns*sizeof(double));
    double *b = (double *) malloc(rows*sizeof(double));    
    double **a = (double **) malloc(rows*sizeof(double*));    
    for (i=0; i<rows; i++)
    {
        a[i] = (double *) malloc(columns*sizeof(double));
    }

    if (rows==3)
    {
        /*
        * Small example should result in
        *  b[0] =  0.0
        *  b[1] =  1.0
        *  b[2] =  1.5
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
    if (rows==3)
    {
        printmatrix(a,rows,columns);
        printvector(x,columns);
    }
    
    /*
     * This is the main part, doing the actual computation
     */
    #pragma omp parallel for private(i, j, sum)
    for (i=0; i<rows; i++)
    {
                
        sum = 0.0;
        for (j=0; j<columns; j++)
        {
                sum += a[i][j]*x[j];
        }
        b[i] = sum;
        
    }
    
    // output the result
    if (rows==3)
    {
        printvector(b, rows);
    }
    else
    {
        printf("First element of b: %i \n", (int) b[0]);
        printf("Last element of b: %i \n",  (int) b[rows-1]);
    }
    
}
