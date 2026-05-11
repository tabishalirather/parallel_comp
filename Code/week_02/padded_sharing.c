#include <omp.h>
#include <stdio.h>

struct PaddedInt {
    int value;
    char padding[64 - 4]; // Assuming 4 threads here
};

int main() {
    omp_set_num_threads(4);
    int num_threads = omp_get_max_threads();
    struct PaddedInt data[num_threads];

    // Initialize the array
    for (int i = 0; i < num_threads; ++i) {
        data[i].value = 0;
    }

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        for (int i = 0; i < 1e7; ++i) {
            // Each thread modifies its own unique padded element
            data[thread_id].value++;
        }
    }

    // Print the results
    for (int i = 0; i < num_threads; ++i) {
        printf("data[%d] = %d\n", i, data[i].value);
    }

    return 0;
}