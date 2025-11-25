#!/bin/bash
# Valgrind Memcheck profiler

if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <results_dir> <executable> [executable args...]"
  exit 1
fi

RESULTS_DIR="$1"
EXECUTABLE="$2"
shift 2  # Shift away the first two parameters, leaving only executable args

# Ensure the output directory exists
mkdir -p "$RESULTS_DIR"

# Run Valgrind Memcheck with the provided executable and arguments
echo "Running Valgrind Memcheck..."
valgrind --leak-check=full \
  --show-leak-kinds=all \
  --num-callers=40 \
  "$EXECUTABLE" "$@" 2> "$RESULTS_DIR/memcheck.txt"

echo "Results:"
echo "  Raw output:    $RESULTS_DIR/memcheck.txt"