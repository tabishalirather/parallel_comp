# Parallel Computing: OpenMP & MPI Guide

A comprehensive guide to parallel programming concepts with OpenMP and MPI. This repository covers fundamental concepts, common pitfalls, and practical examples.

---

## Table of Contents

1. [Structured Blocks](#structured-blocks)
2. [OpenMP Basics](#openmp-basics)
3. [Directives vs Clauses](#directives-vs-clauses)
4. [Private and Reduction Clauses](#private-and-reduction-clauses)
5. [False Sharing and Cache Lines](#false-sharing-and-cache-lines)
6. [Random Number Generation in Parallel](#random-number-generation-in-parallel)
7. [OpenMP vs MPI](#openmp-vs-mpi)
8. [Monte Carlo Example](#monte-carlo-example)
9. [Compilation](#compilation)

---

## Structured Blocks

### Definition

A **structured block** is a single entry, single exit block of code governed by an OpenMP directive.

### Valid Structured Blocks

✅ Single statement:
```c
#pragma omp parallel
    printf("Hello from thread %d\n", omp_get_thread_num());
```

✅ Compound statement (block):
```c
#pragma omp parallel
{
    int id = omp_get_thread_num();
    printf("Thread %d\n", id);
}
```

### Invalid Structured Blocks

❌ Code that exits the block early:
```c
#pragma omp parallel
{
    if (error) return;  // INVALID - exits function
}
```

### Key Concept

With `#pragma omp parallel`, **the structured block is duplicated and executed by all threads simultaneously**. If you have 4 threads set, the code block runs 4 times in parallel.

---

## OpenMP Basics

### What is OpenMP?

OpenMP allows parallel programming on **shared memory systems** (single machine with multiple cores) using simple compiler directives.

### Basic Structure

```c
#pragma omp parallel num_threads(4)
{
    // Code here runs on 4 threads simultaneously
    printf("Thread %d\n", omp_get_thread_num());
}
```

### Setting Number of Threads

Three methods:

**1. Directive clause:**
```c
#pragma omp parallel num_threads(4)
```

**2. Function call:**
```c
omp_set_num_threads(4);
#pragma omp parallel
```

**3. Environment variable:**
```bash
export OMP_NUM_THREADS=4
./program
```

---

## Directives vs Clauses

### Directive

The main OpenMP statement:
```c
#pragma omp parallel for
         ^^^^^^^^^^^^^^^
         This is the DIRECTIVE
```

### Clause

Optional modifier that changes directive behavior:
```c
#pragma omp parallel for num_threads(4) reduction(+:sum)
                        ^^^^^^^^^^^^^^  ^^^^^^^^^^^^^^^^
                        Clause           Clause
```

### Common Clauses

| Clause | Purpose |
|--------|---------|
| `num_threads(n)` | Set number of threads |
| `private(var)` | Make variable thread-local |
| `shared(var)` | Make variable shared (default) |
| `reduction(op:var)` | Combine results with operator |
| `schedule(type, chunk)` | Distribute loop iterations |
| `collapse(n)` | Nest multiple loops |
| `if(condition)` | Conditional parallelization |

---

## Private and Reduction Clauses

### Variables Declared Inside Parallel Block

```c
#pragma omp parallel
{
    int x = 5;  // Automatically private!
    // Each thread gets own x
}
```

**Rule:** Variables declared inside the parallel block are **automatically private** without needing to declare it.

### `private()` Clause

Use for **temporary variables that don't need combining**:

```c
int sum = 0;  // Outside = shared

#pragma omp parallel for private(temp)
for (int i = 0; i < N; i++) {
    int temp = some_calculation();  // Each thread has own temp
    sum += temp;  // But sum is still shared!
}
```

**Problem:** Without `reduction()`, sum will have race conditions!

### `reduction()` Clause

Use when **multiple threads modify a variable AND you need the combined result**:

```c
int sum = 0;

#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < N; i++) {
    sum += arr[i];  // Each thread accumulates privately
}

// After loop: sum = total from all threads
printf("Total: %d\n", sum);  // Correct!
```

### How Reduction Works

1. **Create private copies:** Each thread gets own `sum` initialized to 0
2. **Accumulate independently:** Thread 0: sum=1000, Thread 1: sum=1500, etc.
3. **Combine with operator:** Final sum = 1000 + 1500 + ... = total

### Reduction Operators

```c
reduction(+:var)      // Add all
reduction(*:var)      // Multiply all
reduction(-:var)      // Subtract all
reduction(max:var)    // Keep maximum
reduction(min:var)    // Keep minimum
reduction(&:var)      // Bitwise AND
reduction(|:var)      // Bitwise OR
reduction(^:var)      // Bitwise XOR
```

### Decision Guide

| Scenario | Use |
|----------|-----|
| Temporary variable (not needed after) | `private()` |
| Accumulate sum/count | `reduction(+:var)` |
| Find max/min | `reduction(max:var)` or `reduction(min:var)` |
| Loop variable | Nothing (auto-private) |
| Shared by all threads | `shared()` or default |

---

## False Sharing and Cache Lines

### Cache Line Basics

A **cache line** is the smallest unit of data transferred between main memory and CPU cache.

**Typical size:** 64 bytes

### The Problem: False Sharing

When multiple threads modify **different variables** on the **same cache line**, they invalidate each other's cache copies, causing performance degradation.

```c
int sum[4];  // All 4 integers on SAME cache line!

#pragma omp parallel
{
    int id = omp_get_thread_num();
    for (int i = 0; i < 1000000; i++) {
        sum[id]++;  // All threads fight over same cache line
    }
}
```

### Bathroom Analogy

```
❌ False Sharing (1 Bathroom):
Thread 0: Brush teeth (2 min)
Thread 1: Shower (10 min)
Total: 12 min (sequential!)

✅ No False Sharing (4 Bathrooms):
Thread 0: Brush teeth (2 min)
Thread 1: Shower (10 min)
Thread 2: Wash hands (1 min)
Thread 3: Use toilet (3 min)
Total: 10 min (parallel!)
```

### Solution 1: Padding

```c
struct PaddedSum {
    int value;
    int padding[15];  // Pad to 64 bytes
};

PaddedSum sum[4];  // Each on different cache line
```

### Solution 2: Reduction (Better)

```c
int sum = 0;

#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < 1000000; i++) {
    sum++;
}
```

### Solution 3: Private Variables + Manual Combine

```c
#pragma omp parallel
{
    int local_sum = 0;
    #pragma omp for
    for (int i = 0; i < N; i++) {
        local_sum += arr[i];  // No cache conflicts
    }
    #pragma omp critical
    total_sum += local_sum;   // Combine at end
}
```

---

## Random Number Generation in Parallel

### The Problem with `rand()`

```c
#pragma omp parallel for
for (int i = 0; i < 1e9; i++) {
    double x = (double)rand() / RAND_MAX;
}
```

**Issues:**
- ❌ `rand()` is NOT thread-safe
- ❌ All threads access shared internal state
- ❌ Serialization → bottleneck
- ❌ Unpredictable results

### Solution: Use `erand48()`

`erand48()` is **thread-safe** and returns doubles directly (0.0 to 1.0).

```c
#include <stdlib.h>

#pragma omp parallel
{
    unsigned short state[3];
    unsigned short seed = omp_get_thread_num();
    state[0] = seed;
    state[1] = seed + 1;
    state[2] = seed + 2;
    
    #pragma omp for
    for (int i = 0; i < 1000000; i++) {
        double x = erand48(state);  // 0.0 to 1.0
        double y = erand48(state);
        
        if (x*x + y*y <= 1.0) {
            // Inside quarter circle
        }
    }
}
```

### Why `state[3]`?

- `erand48()` uses a 48-bit random generator
- 3 `unsigned short` values = 48 bits
- Each thread needs its own state (different seed)

### Key Points

- ✅ Initialize state **once per thread** (outside loop)
- ✅ Each thread gets different seed (use `omp_get_thread_num()`)
- ✅ State updates automatically with each call
- ✅ Thread-safe!

---

## OpenMP vs MPI

### Quick Comparison

| Feature | OpenMP | MPI |
|---------|--------|-----|
| **Hardware** | Shared memory (1 machine) | Distributed memory (many machines) |
| **Model** | Threads on same CPU | Separate processes |
| **Communication** | Threads share variables | Message passing |
| **Ease** | Easier (pragmas) | Harder (explicit messages) |
| **Speed** | Fast (shared memory) | Slower (network) |
| **Scalability** | Limited to one machine | Unlimited (cluster/supercomputer) |

### When to Use OpenMP

- Single machine with multiple cores
- Shared memory available
- Quick parallelization
- Examples: Image processing, matrix operations, Monte Carlo on one machine

```c
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    a[i] = b[i] + c[i];
}
```

### When to Use MPI

- Multiple machines (cluster/supercomputer)
- Distributed memory (each process has own memory)
- Need to send/receive messages between processes
- Examples: Large simulations, weather models, distributed machine learning

```bash
mpirun -n 4 ./program.out
// Runs 4 separate processes
```

### Visual Difference

**OpenMP (1 Machine):**
```
┌──────────────────────┐
│ CPU Core 0           │
│ CPU Core 1  ← Shared memory
│ CPU Core 2           │
│ CPU Core 3           │
└──────────────────────┘
```

**MPI (Multiple Machines):**
```
┌──────────────┐  ┌──────────────┐
│ Machine 1    │  │ Machine 2    │
│ Process 0    │  │ Process 1    │
└──────────────┘  └──────────────┘
     ↓ Messages over network ↓
```

---

## Monte Carlo Example

### The Algorithm

Estimate π by throwing "darts" at a quarter circle:

1. Generate random points in [0,1] × [0,1]
2. Count how many land inside the quarter circle (x² + y² ≤ 1)
3. Ratio of hits to total ≈ π/4
4. Therefore: π ≈ 4 × (hits / total)

### Complete OpenMP Implementation

```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int hits = 0;
    int not_hits = 0;
    int attempts = 100000000;
    
    printf("Running Monte Carlo with %d attempts\n", attempts);
    
    #pragma omp parallel
    {
        // Each thread gets own random state
        unsigned short state[3];
        unsigned short seed = omp_get_thread_num();
        state[0] = seed;
        state[1] = seed + 1;
        state[2] = seed + 2;
        
        #pragma omp for reduction(+:hits) reduction(+:not_hits)
        for (int i = 0; i < attempts; i++) {
            // Generate random 0.0 to 1.0
            double x_rand = erand48(state);
            double y_rand = erand48(state);
            
            // Check if inside quarter circle
            if (x_rand * x_rand + y_rand * y_rand <= 1.0) {
                hits++;
            } else {
                not_hits++;
            }
        }
    }
    
    // Calculate π estimate
    double ratio = (double)hits / attempts;
    double pi_estimate = 4.0 * ratio;
    
    printf("Hits: %d\n", hits);
    printf("Misses: %d\n", not_hits);
    printf("Ratio: %f\n", ratio);
    printf("π ≈ %f\n", pi_estimate);
    
    return 0;
}
```

### Key Concepts Used

- ✅ `#pragma omp parallel` - Create thread team
- ✅ `unsigned short state[3]` - Auto-private (declared inside)
- ✅ `#pragma omp for` - Distribute loop iterations
- ✅ `reduction(+:hits)` - Accumulate hits from all threads
- ✅ `erand48(state)` - Thread-safe random numbers
- ✅ `omp_get_thread_num()` - Get current thread ID

---

## Compilation

### Compile OpenMP Program

```bash
gcc monte_carlo.c -o monte_carlo.out -lm -fopenmp
```

**Flags:**
- `-lm` : Link math library (for any math functions)
- `-fopenmp` : Enable OpenMP support

### Run with Specific Thread Count

```bash
export OMP_NUM_THREADS=4
./monte_carlo.out
```

Or specify in code:
```c
omp_set_num_threads(4);
```

### With MPI (If Using MPI)

```bash
mpicc monte_carlo.c -o monte_carlo.out -lm -fopenmp
mpirun -n 4 ./monte_carlo.out
```

---

## Common Mistakes & Solutions

### Mistake 1: Integer Division with Random Numbers

```c
// ❌ WRONG: Always produces 0
double x = (double)((rand() % 10) / 10);

// ✅ CORRECT: Cast to double BEFORE division
double x = (double)rand() / RAND_MAX;
```

### Mistake 2: Wrong `printf()` Format

```c
double x = 3.14;

// ❌ WRONG: %d expects int
printf("%d\n", x);

// ✅ CORRECT: %f for double
printf("%f\n", x);
```

### Mistake 3: Forgetting `-lm` Flag

```c
double result = pow(2, 3);  // Needs math library
```

```bash
# ❌ WRONG: Will get "undefined reference to `pow'"
gcc program.c -o program.out

# ✅ CORRECT: Add -lm flag
gcc program.c -o program.out -lm
```

### Mistake 4: Race Condition (Shared Variable)

```c
int sum = 0;

#pragma omp parallel for
for (int i = 0; i < N; i++) {
    sum += arr[i];  // ❌ RACE CONDITION!
}

// Fix: Use reduction
#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < N; i++) {
    sum += arr[i];  // ✅ Thread-safe
}
```

### Mistake 5: Using Non-Thread-Safe `rand()`

```c
// ❌ WRONG: Thread-unsafe, slow, wrong results
double x = (double)rand() / RAND_MAX;

// ✅ CORRECT: Use erand48()
double x = erand48(state);
```

---

## Quick Reference

### Common Pragmas

```c
// Parallel region
#pragma omp parallel num_threads(4)

// Parallel loop with work distribution
#pragma omp parallel for

// Combine multiple reductions
#pragma omp parallel for reduction(+:hits) reduction(+:misses)

// Private variables
#pragma omp parallel for private(x, y)

// Critical section (mutual exclusion)
#pragma omp critical
    shared_var++;

// Barrier synchronization
#pragma omp barrier
```

### Common Functions

```c
int omp_get_num_threads();      // Total threads
int omp_get_thread_num();       // Current thread ID
void omp_set_num_threads(n);    // Set thread count
double erand48(unsigned short state[3]);  // Random 0.0-1.0
```

---

## Resources

- [OpenMP Official Docs](https://www.openmp.org/)
- `man erand48` - Info about erand48()
- `man omp` - Info about OpenMP functions
- Course materials: See PDF files in this repo

---

## Summary

**OpenMP Basics:**
- Use `#pragma omp` directives for parallel programming
- Structured blocks run on all threads simultaneously
- Use `reduction()` to combine results from multiple threads
- Use `private()` for thread-local temporary variables
- Variables declared inside parallel block are auto-private

**Parallel Programming Best Practices:**
- Use thread-safe random generators (`erand48()`)
- Avoid false sharing (separate cache lines)
- Always use `reduction()` when accumulating values
- Compile with `-fopenmp` flag
- Test with different thread counts

**When to Use:**
- **OpenMP:** Single machine, shared memory
- **MPI:** Multiple machines, distributed memory

