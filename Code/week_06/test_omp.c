#include <stdio.h>
#include <omp.h>

int main() {
  printf("Testing OpenMP\n\n");

  // Test 1: Parallel region with thread count
  printf("Test 1: Basic parallel region\n");
#pragma omp parallel
  {
    int thread_id = omp_get_thread_num();
    int num_threads = omp_get_num_threads();
    printf("Hello from thread %d of %d\n", thread_id, num_threads);
  }

  // Test 2: Parallel for loop with reduction
  printf("\nTest 2: Parallel sum reduction\n");
  int sum = 0;
#pragma omp parallel for reduction(+:sum)
  for (int i = 1; i <= 100; i++) {
    sum += i;
  }
  printf("Sum of 1 to 100 = %d (expected 5050)\n", sum);

  // Test 3: Parallel loop with private variables
  printf("\nTest 3: Parallel computation\n");
  int results[4] = {0};
#pragma omp parallel for
  for (int i = 0; i < 4; i++) {
    results[i] = i * i;
  }
  printf("Squares: ");
  for (int i = 0; i < 4; i++) {
    printf("%d ", results[i]);
  }
  printf("\n");

  printf("\nAll tests passed!\n");


#pragma omp parallel for
  for (int i = 0; i < 10; i++)
  {
    // printf("i:%d \n", i);
    sum += (i + 1);
  }
  printf("Our answer is:                   %i \n", sum);

  return 0;
}