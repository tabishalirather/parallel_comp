// MPI library
#include "mpi.h"

// Needed to write output to terminal
#include <stdio.h>

// provides srand48 and drand48
#include <stdlib.h>

// provides M_PI constant
#include <math.h>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

int main(int argc, char **argv)
{
    double x, y, pi;

    int i, hits = 0;

    // number of "darts" to throw
    int attempts = 20000000;

    // MPI related variables
    int myid, world_size; // holds process's rank id

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Work out how many attempts each process has to do
    int myattempts = (int)attempts / world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &myid); // rank/id of current process

    srand48(myid); // Each process uses a different seed: otherwise they all produce the same random numbers

    for (i = 0; i < myattempts; ++i)
    {
        x = (double)drand48();
        y = (double)drand48();

        if ((x * x + y * y) <= 1)
        {
            hits++; // count the hit
        }
    }

    int total_hits, total_attempts;

    // Sum up all hits from all processes
    MPI_Reduce(&hits,       // variable storing the data to be processed
               &total_hits, // where to store the result of the operation
               1,
               MPI_INT, // data type to be processed
               MPI_SUM, // arithmetic operation to perform
               0,
               MPI_COMM_WORLD);

    // Sum up all attempts from all processes
    MPI_Reduce(&myattempts,
               &total_attempts,
               1,
               MPI_INT,
               MPI_SUM,
               0,
               MPI_COMM_WORLD);

    /*
     * Since only the process with rank 0 has executed the MPI_Reduce function, only this process has to actual approximation.
     * If we change the number if in if statement, we will not get the same result. Set it to one and see what happens.
     */
    if (myid == 0)
    {
        pi = ((double)total_hits / (double)total_attempts) * 4.0;
        printf("Pi from GNU C lib: %f \n", M_PI);
        printf("Approximate Pi:    %f\n", pi);
        printf("Total number of hits:     %i\n", total_hits);
        printf("Total number of attempts: %i\n", total_attempts);
    }

    MPI_Finalize();
    return 0;
}
