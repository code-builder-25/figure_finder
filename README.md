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
cd build && ./figure_finder_app data/matrix_120x100_500.txt

```

### Run Tools
```bash
# Run clang-format for code formatting
cd build && ninja clang-format

# Run clang-tidy for code quality, bug detection, and best-practice enforcement
cd build && ninja clang-tidy
# or
clang-tidy ../src/main.cpp -- -I../include
# or
run-clang-tidy -p build
```

## Data

Operates on matrices of 1s and 0s, where 1s represent figure parts and 0s represent empty space. Matrices are stored as `.txt` files.

### Example Matrices

Refer to the `data` folder for sample matrices.

### Matrix Generation

Generate matrices of custom size, figure count, and average figure size using the following script:
```bash
python3 scripts/generate_matrix.py --rows 2000 --cols 1500 --figures 50000 --outfile data/matrix.txt
```
