# Parallel Triangle Counting

A C++17 multithreaded implementation of an algorithm for counting the number of triangles that can be formed from a given set of stick lengths.

**Concurrent Programming Project — Yahor Yasinski**

## Algorithm

The program first sorts the stick lengths and then uses a two-pointer approach to count valid triangles, achieving **O(n²)** time complexity.

Since the computation for each longest-side index is independent, the workload can be distributed across multiple threads.

## Parallelization Strategies

Four execution modes were implemented:

- **seq** — sequential execution.
- **equal** — equal-sized ranges of indices assigned to threads.
- **balanced** — static partitioning based on the estimated number of operations.
- **dynamic** — dynamic scheduling using an atomic counter and chunks of 32 indices.

Each thread maintains its own partial result, which is combined after all threads finish.

## Build and Run

```bash
make
./triangles dynamic 16 < input.txt
```

The first argument specifies the scheduling mode, and the second specifies the number of threads.

To generate input and run correctness tests:

```bash
echo 200000 | ./generator > input.txt
python3 run_tests.py
```

## Performance Results

Benchmarks with **200,000 sticks**:

| Mode | Local (16 threads) | Server (96 threads) |
|---|---:|---:|
| Sequential | 30.639 s | 38.201 s |
| Equal | 3.414 s | 0.843 s |
| Balanced | 2.303 s | 0.520 s |
| Dynamic | **2.190 s** | **0.447 s** |

The dynamic strategy achieved:

- **14.0× speedup** on the local machine.
- **85.5× speedup** on the server.
- **87.4× speedup** on the server with 400,000 sticks.

Speedups are calculated relative to sequential execution on the same machine. Measurements include input reading and sorting and represent individual runs.

## Conclusions

Multithreading significantly improves performance for this computationally intensive problem. However, the scheduling strategy is equally important.

Dynamic scheduling achieved the best results by reducing workload imbalance between threads, outperforming both static partitioning strategies in the measured configurations.
