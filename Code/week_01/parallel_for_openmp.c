// Include the OpenMP header
#include <omp.h>

// This we only need to write output
#include <stdio.h>

int main(int argc, char **argv) {
  int sum = 0;

  int n = 5e+6;

#pragma omp parallel
  {

  #pragma omp for reduction(+:sum)
      for (int i = 0; i < n; i++) {
        sum += (i + 1);
    }
  }
  printf("\n");
  printf("Gauss says, the answer should be %i \n", (int) (n / 2) * (n + 1));
  puts("The key idea is to prevent race condition on sum by adding clause reduction(+:sum)");
  printf("Our answer is:                   %i \n", sum);
}
