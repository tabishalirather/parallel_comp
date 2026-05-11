#include <omp.h>
#include <stdio.h>

int main() {
    int num_threads = omp_get_max_threads();
    // Assuming an int is 4 bytes, and a cache line is typically 64 bytes, each cache line can hold 16 ints.
    // To demonstrate false sharing, we use an array where each element is likely to be on the same cache line.
    int data[num_threads];

    // Initialize the array
    for (int i = 0; i < num_threads; ++i) {
        data[i] = 0;
    }

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        for (int i = 0; i < 1e7; ++i) {
            // Each thread modifies its own unique element, but these modifications can lead to false sharing
            data[thread_id]++;
        }
    }

    // Print the results
    for (int i = 0; i < num_threads; ++i) {
        printf("data[%d] = %d\n", i, data[i]);
    }

    return 0;
}