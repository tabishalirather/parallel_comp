// Include the OpenMP header
#include <omp.h>

// This we only need to write output
#include <stdio.h>

int main(int argc, char **argv)
{
    int sum = 0;

    int n = 5e+6;

    // This is what the reduction does intrinsically
    // #pragma omp parallel default(none) shared(sum, n)
    // {
    //     int local_sum = 0;
    //     #pragma omp for
    //     for (int i = 0; i < n; i++)
    //     {
    //         local_sum += (i + 1);
    //     }

    //     #pragma omp atomic
    //     sum = sum + local_sum;
    // }

    // the reduction makes sum automatically private
    #pragma omp parallel reduction(+ : sum)
    #pragma omp for
    for (int i = 0; i < n; i++)
    {
        sum += (i + 1);
    }

    printf("\n");
    printf("Gauss says, the answer should be %i \n", (int)(n / 2) * (n + 1));
    printf("Our answer is:                   %i \n", sum);
}
