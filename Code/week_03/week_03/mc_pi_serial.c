// Needed to write output to terminal
#include <stdio.h>

// provides srand48 and drand48
#include <stdlib.h>

// provides M_PI constant
#include <math.h>

int main(int argc, char **argv)
{
    double x, y, pi;

    int hits, i;

    // number of "darts" to throw
    int attempts = 10000000;

    /*
     * This will produce the same random numbers in every run. Better to use time to seed but this is not important right now.
     */
    srand48(0);

    for (i = 0; i < attempts; i++)
    {

        /*
         *  Throw the dart by randomly generating a x and y coordinate
         */
        x = (double)drand48();
        y = (double)drand48();

        /*
         *  If (x,y) is less than 1 away from the origin, the dart hit the quarter circle
         */
        if ((x * x + y * y) <= 1) // Technically we need to use a sqrt here, but sqrt(x) < 1 if and only if x < 1 here
        {
            hits++; // count the hit
        }
    }

    /*
     * Number of hits / number of attempts approximates the area of the quarter circle.
     * Multiply by 4 to get area of full circle.
     */
    pi = 4.0 * ((double)hits / (double)attempts);

    /*
     * Print out our results to the terminal
     */
    printf("Pi from GNU C lib: %f \n", M_PI);
    printf("Approximate Pi: %f\n", pi);
    printf("Total number of hits:     %i\n", hits);
    printf("Total number of attempts: %i\n", attempts);
}
