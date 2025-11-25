#!/bin/bash
# Perf profiler
# apt update && apt install linux-tools-generic linux-cloud-tools-generic
# /usr/lib/linux-tools-$(uname -r)/perf

# Compile your app with debug info: -g -O2
# Optionally include inlining info: -fno-omit-frame-pointer

if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <results_dir> <executable> [executable args...]"
  exit 1
fi

RESULTS_DIR="$1"
EXECUTABLE="$2"
shift 2  # Shift away the first two parameters, leaving only executable args

# Ensure the output directory exists
mkdir -p "$RESULTS_DIR"

# Run perf stat with the provided executable and arguments
echo "Running perf stat (CPU Cache & Performance Counters)..."
perf stat -e cycles,instructions,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-load-misses,LLC-store-misses \
  -o $RESULTS_DIR/perf_stat.txt \
  "$EXECUTABLE" "$@" \
  2>&1 || echo "Note: perf requires privileged mode"

echo "Results:"
echo "  Raw output:    $RESULTS_DIR/perf_stat.txt"