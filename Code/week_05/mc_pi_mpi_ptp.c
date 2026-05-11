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

    if (myid == 0)
    {
        /*
         *
         * Note that if you accidentally started at i=0, the 0 rank process would post an MPI_RECV but no process would post a corresponding MPI_SEND
         * The result is a deadlock. Try it and see what happens!
         * Remember, CTRL + X + C terminates the program currently running in your terminal ;)
         */
        for (i = 1; i < world_size; i++)
        {
            /*
             * The process with rank zero has to post MPI_RECV for every send by a process with rank > 0.
             */
            int hits_recv = 0;
            int attempts_recv = 0;
            MPI_Recv(&hits_recv,
                     1,
                     MPI_INT,
                     i,
                     0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            hits += hits_recv;

            MPI_Recv(&attempts_recv,
                     1,
                     MPI_INT,
                     i,
                     0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            myattempts += attempts_recv;
        }
    }
    /*
     * All processes with rank > 0 send data to the process with rank 0.
     */
    else
    {
        MPI_Send(&hits,
                 1,
                 MPI_INT,
                 0,
                 0,
                 MPI_COMM_WORLD);

        MPI_Send(&myattempts,
                 1,
                 MPI_INT,
                 0,
                 0,
                 MPI_COMM_WORLD);
    }

    if (myid == 0)
    {
        pi = ((double)hits / (double)attempts) * 4.0;
        printf("Pi from GNU C lib: %f \n", M_PI);
        printf("Approximate Pi:    %f\n", pi);
        printf("Total number of hits:     %i\n", hits);
        printf("Total number of attempts: %i\n", myattempts);
    }

    MPI_Finalize();
    return 0;
}
