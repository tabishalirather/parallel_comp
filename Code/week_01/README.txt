IMPORTANT: The GCC compiler and OpenMPI library must be installed on your system.

a) Type 

    gcc serial_for.c -o serial_for.out

to compile the code and then

    ./serial_for.out
    
to run it. 

b) To compile the OpenMP program, type

    gcc -fopenmp parallel_for_openmp.c -o parallel_for_openmp.out
    
The number of threads is set via an environment variable. Type

    export OMP_NUM_THREADS=2
    
into your terminal. After that, run

    ./parallel_for_openmp.out   
    
in the same terminal where you set the OMP_NUM_THREADS variable.

