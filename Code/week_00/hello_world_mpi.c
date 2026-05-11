// This needs to be included to make the MPI library available in your program
#include <mpi.h>

// This we only need to write output
#include <stdio.h>

int main(int argc, char **argv) {
  // Integer to store the number of MPI processes launched
  int world_size;

  // The rank ("number") of the current process: from 0 to world_size-1
  int world_rank;

  // Initialize the MPI environment: we need no input parameters, hence NULL
  MPI_Init(NULL, NULL);

  // MPI_Comm_Size is an MPI command that determines the number of processes. This number
  // is set by the -n flag in mpirun. This number is the same for all processes.
  //This modifies the variable world_size
  //Total number of processes that will run
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process: note that every process will be running a copy of this program
  // so in every process, world_rank will have a different value
  //This modifies the variable world_rank
  // Each spin off of the thread assigns its value to it.
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Print "Hello World". Note that every process will print this, so you will see world_size many messages in your terminal
  printf("Hello world says rank %d out of %d processes \n", world_rank, world_size);

  // Finalize the MPI environment.
  MPI_Finalize();
}
