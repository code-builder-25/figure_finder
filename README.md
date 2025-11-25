# figure_finder

Detects and counts figures in a boolean matrix using cell-side connectivity. Implemented in C++.

## Quick Start

### Requirements

- C++20 compatible compiler (GCC 11+, Clang 14+)
- CMake 3.16+
- Ninja (optional, recommended)

#### Installation

```bash
apt-get update && apt-get install -y cmake ninja-build git python3 clang-14
```

### Build

```bash
mkdir -p build

# Configure
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G Ninja

# Build
cmake --build build -j
# or
cd build && ninja
```

### Run tests
```bash
# Run tests
ctest --test-dir build --output-on-failure
# or
cd build && ninja run-tests
```

### Run application
```bash
# Run figure finder application
cd build && ./bin/figure_finder_app data/matrix_120x100_500.txt
```

### Run Tools
```bash
# Run clang-format for code formatting
cd build && ninja clang-format

# Run clang-tidy for code quality, bug detection, and best-practice enforcement
cd build && ninja clang-tidy
# or
clang-tidy ../src/main.cpp -- -I../include
# orclang
run-clang-tidy -p build
```

### Run benchmarks
```bash
# Run compare all algorithms
cd build && ./bin/compare_all
# or
cd build && ninja run-compare-all

# Run CPU + memory benchmark for all algorithms
cd build && ./bin/bench_with_memory
```

### Run sanitizers
```bash
# Run Asan + Ubsan
./scripts/sanitizers/run_asan.sh figure_finder_app data/matrix_2000x1500_50K.txt
# Run Tsan
./scripts/sanitizers/run_tsan.sh figure_finder_app data/matrix_2000x1500_50K.txt
```

### Run memory profilers
```bash
mkdir -p profiling_results
# Run heaptrack
apt-get update && apt-get install heaptrack heaptrack-gui
./scripts/profilers/heaptrack/run_heaptrack.sh profiling_results ./build/bin/figure_finder_app data/matrix_2000x1500_50K.txt
./scripts/profilers/heaptrack/analyze_heaptrack.sh profiling_results 100
./scripts/profilers/heaptrack/visualize_heaptrack.sh profiling_results

# Run Valgrind Massif
apt-get update && apt-get install valgrind massif-visualizer
./scripts/profilers/massif/run_massif.sh profiling_results ./build/bin/figure_finder_app data/matrix_2000x1500_50K.txt
./scripts/profilers/massif/analyze_massif.sh profiling_results 500
./scripts/profilers/massif/visualize_massif.sh profiling_results ./build/bin/figure_finder_app data/matrix_2000x1500_50K.txt

# Run Valgrind Memcheck
apt-get update && apt-get install valgrind
./scripts/profilers/memcheck/run_memcheck.sh profiling_results ./build/bin/figure_finder_app data/matrix_2000x1500_50K.txt
./scripts/profilers/memcheck/analyze_memcheck.sh profiling_results

# Run Perf
apt-get update && apt-get install linux-tools-generic linux-cloud-tools-generic
sudo sysctl kernel.perf_event_paranoid=0 # fix perf Permission Denied on host
./scripts/profilers/perf/run_perf.sh profiling_results ./build/bin/figure_finder_app data/matrix_2000x1500_50K.txt
./scripts/profilers/perf/analyze_perf.sh profiling_results
./scripts/profilers/perf/visualize_perf.sh profiling_results ./build/bin/figure_finder_app data/matrix_2000x1500_50K.txt
```

### References

- [Google Benchmark Docs](https://github.com/google/benchmark)
- [Valgrind Manual](https://valgrind.org/docs/manual/ms-manual.html)
- [Heaptrack GitHub](https://github.com/KDE/heaptrack)
- [Linux perf Examples](https://www.brendangregg.com/perf.html)
- [Sanitizers (ASan/MSan/TSan)](https://iree.dev/developers/debugging/sanitizers/)

## Data

Operates on matrices of 1s and 0s, where 1s represent figure parts and 0s represent empty space. Matrices are stored as `.txt` files.

### Example Matrices

Refer to the `data` folder for sample matrices.

### Matrix Generation

Generate matrices of custom size, figure count, and average figure size using the following script:
```bash
python3 scripts/generate_matrix.py --rows 2000 --cols 1500 --figures 50000 --outfile data/matrix.txt 
```