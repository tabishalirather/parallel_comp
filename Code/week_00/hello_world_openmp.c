// Include the OpenMP header
#include <omp.h>

// This we only need to write output
#include <stdio.h>
 
int main(int argc, char** argv)
{
    // Begin the parallel part of the code: here, the OpenMP library will spawn a number of threads equal 
    // to the OMP_NUM_THREADS environment variable. Each thread will run the code inside the brackets.
    #pragma omp parallel
    {

        // This integer will store the number (rank) of the current thread
        //identifying number of the current thread.

        int thread_num;
        
        // This integer will store the total number of threads
        //total number of threads we use when running this code.
        int num_threads;
        
        // get the number of the current thread
        thread_num = omp_get_thread_num();
        
        // get the total number of threads
        num_threads = omp_get_num_threads();

        printf("Hello World... from thread number = %d out of %d many threads \n", thread_num, num_threads);
    }
    // Ending of parallel region
    
    // Important: if you ask how many threads there are outside of the parallel region, the answer will be one! 
    printf("Number of threads outside of parallel region: %d \n", omp_get_num_threads());
}
