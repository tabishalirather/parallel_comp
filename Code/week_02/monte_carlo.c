#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
int main() {
  puts("This si monte carlo");
  srand(time(NULL));
  // int RAND_MAX = 5;

  // printf("%f\n", x_rand);
  // printf("%f\n", y_rand);
  int hits = 0;
  int not_hits = 0;
  int  attempts = 1e9;
  for (int i=0; i<attempts; i++) {
    // printf("i is: %d", i);
    double x_rand = (double)rand() / RAND_MAX;
    double y_rand = (double)rand() / RAND_MAX;
    if  ((x_rand * x_rand) + (y_rand * y_rand) <= 1) {
      hits++;
    }else {
      not_hits++;
    }
  }
  double ratio = (double) hits/attempts;
  printf("hits: %d\n", hits);
  printf("ratio is: %f\n", ratio);
  printf("not hits: %d\n", not_hits);
  printf("Estimate of pi is: %f", 4*ratio);


  return -1;
}

