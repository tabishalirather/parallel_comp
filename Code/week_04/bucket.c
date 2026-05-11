// This needs to be included to make the MPI library available in your program
#include <mpi.h>

// This we only need to write output
#include <stdio.h>

int main(int argc, char** argv) {

     // Integer to store the number of MPI processes launched   
    int world_size;
    
    // The rank ("number") of the current process: from 0 to world_size-1
    int world_rank;
    
    int counter;
    
    // Initialize the MPI environment: we need no input parameters, hence NULL
    MPI_Init(NULL, NULL);

    // MPI_Comm_Size is an MPI command that determines the number of processes. This number
    // is set by the -n flag in mpirun/mpiexec. This number is the same for all processes.
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process: note that every process will be running a copy of this program
    // so in every process, world_rank will have a different value
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    if (world_rank==0) counter = 0;        
    else counter = -999; // Set a weird value to see if each process has overwritten it

    // all except the first process have to RECV the counter
    if (world_rank > 0)
    {    
        MPI_Recv(&counter, 1, MPI_INT, world_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        counter += 1; // after receiving the counter, increment by one

    }
    
    // all except the last process have to SEND the counter
    if (world_rank < world_size-1)
    {
        MPI_Send(&counter, 1, MPI_INT, world_rank+1, 0, MPI_COMM_WORLD);
    }
    
    printf("Process %d has counter %d \n", world_rank, counter);

    // Finalize the MPI environment.
    MPI_Finalize();
}
