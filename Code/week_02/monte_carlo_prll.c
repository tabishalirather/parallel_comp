#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int main() {
  puts("This si monte carlo");
  // srand(time(NULL));
  // int RAND_MAX = 5;

  // printf("%f\n", x_rand);
  // printf("%f\n", y_rand);
  int hits = 0;
  int not_hits = 0;
  long attempts = 1e9;
  /*
* Variable inside loop?
  ├─ YES: Do I need combined result?
  │   ├─ YES → reduction(op:var)
  │   └─ NO → private(var)
  │
  └─ NO: Do multiple threads access it?
    ├─ YES → Check for race conditions!
    └─ NO → Doesn't matter
   */
  unsigned short state[3];
  unsigned short seed = 12345;

  // Initialize state
  state[0] = seed;
  state[1] = seed + 1;
  state[2] = seed + 2;
  /*
  * state[3] = Box with 3 slots
    seed = Starting number
    state[0,1,2] = Fill slots with seed, seed+1, seed+2
    erand48(state) = Use the box to make random numbers (and update the box)
   */
  omp_set_num_threads(5);
#pragma omp parallel for reduction(+:hits) reduction(+:not_hits)
  for (int i = 0; i < attempts; i++) {
    // printf("i is: %d", i);
    double x_rand = erand48(state);
    double y_rand = erand48(state);
    if ((x_rand * x_rand) + (y_rand * y_rand) <= 1) {
      hits++;
    } else {
      not_hits++;
    }
  }
  double ratio = (double) hits / attempts;
  printf("hits: %d\n", hits);
  printf("ratio is: %f\n", ratio);
  printf("not hits: %d\n", not_hits);
  printf("Estimate of pi is: %f", 4 * ratio);


  return -1;
}
