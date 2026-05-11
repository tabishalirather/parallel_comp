IMPORTANT: The GCC compiler and OpenMPI library must be installed on your system.

a) Type 

    gcc hello_world.c -o hello_world.out

to compile the code and then

    ./hello_world.out
    
to run it. 

b) To compile the OpenMP program, type

    gcc -fopenmp hello_world_openmp.c -o hello_world_openmp.out
    
The number of threads is set via an environment variable. Type

    export OMP_NUM_THREADS=2
    
into your terminal. After that, run

    ./hello_world_openmp.out   
    
in the same terminal where you set the OMP_NUM_THREADS variable.

c) Type

    mpicc hello_world_mpi.c -o hello_world_mpi.out -lm

to compile the problem (-lm links against the math library) and then

    mpirun -n 2 ./hello_world_mpi.out

to run it. The number after the n is the number of processes launched. Change it and see what happens.

