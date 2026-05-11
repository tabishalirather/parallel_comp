#include <stdio.h>
#include <stdlib.h>

/*********************************** 
Example of matrix allocation in C. 
Allocates memory for a 3x2 matrix and fills it with values.

Run with: 
gcc matrix.c -o matrix.out
./matrix 
***************************************/

int main(void)
{
    int rows = 3;
    int cols = 2;

    double** A = malloc(rows*sizeof(double*)); // for production code, check if malloc returns NULL
    if (A==NULL) {
        puts("Something is wrong");
    }
    for (int i = 0; i < rows; i++) {
        A[i] = malloc(cols*sizeof(double)); // access to i'th element (pointer to i'th row) of A with A[i]
    }

    // Fill the matrix with some arbitrary values
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            A[i][j] = 4*i + j; // access to the matrix elements using A[i][j]
            printf("A[%d][%d] = %f\n", i, j, A[i][j]);
        }
    }

    // Compute the storage size of the matrix in bytes
    int size = rows * cols * sizeof(double);
    printf("Storage size of the matrix: %d bytes\n", size);

    // Don't forget to free the allocated memory
    for (int i = 0; i < rows; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}
