// This needs to be included to make the MPI library available in your program
#include <mpi.h>

// This we only need to write output
#include <stdio.h>

int main(int argc, char **argv)
{

    int n = 5e+6;

    // Integer to store the number of MPI processes launched
    int world_size;

    // The rank ("number") of the current process: from 0 to world_size-1
    int world_rank;

    // Initialize the MPI environment: we need no input parameters, hence NULL
    MPI_Init(NULL, NULL);

    // MPI_Comm_Size is an MPI command that determines the number of processes. This number
    // is set by the -n flag in mpirun. This number is the same for all processes.
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process: note that every process will be running a copy of this program
    // so in every process, world_rank will have a different value
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int n_per_process = n / world_size; // careful, what is the assumption here?

    int my_n_start = world_rank * n_per_process + 1;

    int my_n_end = (world_rank + 1) * n_per_process;

    int my_sum = 0;

    int total_sum = 0;

    printf("Process %d counts from %d to %d \n", world_rank, my_n_start, my_n_end);

    for (int i = my_n_start; i <= my_n_end; i++)
    {
        my_sum += i;
    }

    MPI_Reduce(&my_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        printf("\n");
        printf("Gauss says, the answer should be %i \n", (int)(n / 2) * (n + 1));
        printf("Our answer is:                   %i \n", total_sum);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}
