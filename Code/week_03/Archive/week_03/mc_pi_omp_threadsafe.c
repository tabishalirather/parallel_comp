// Include the OpenMP header
#include <omp.h>

// This we only need to write output
#include <stdio.h>

// provides RAND_MAX
#include <stdlib.h>

// provides M_PI constant
#include <math.h>

int main(int argc, char **argv)
{
    double x, y, pi;

    int hits = 0, i;

    int attempts = 10000000;

    unsigned short xi[3];

    #pragma omp parallel
    {

        // Intialize the random number generator
        xi[0] = 1;
        xi[1] = 1;
        xi[2] = omp_get_thread_num();

        #pragma omp for firstprivate(xi) private(x, y) reduction(+ : hits)
        for (i = 0; i < attempts; i++)
        {

            x = (double)erand48(xi);
            y = (double)erand48(xi);

            if ((x * x + y * y) <= 1)
            {
                hits++;
            }
        }
    }

    pi = 4.0 * ((double)hits / (double)attempts);
    
    /*
     * Print out the results
     */
    printf("Pi from GNU C lib: %f \n", M_PI);
    printf("Approximate Pi: %f\n", pi);
    printf("Total number of hits:     %i\n", hits);
    printf("Total number of attempts: %i\n", attempts);
}
