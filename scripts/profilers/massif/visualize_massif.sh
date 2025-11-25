#!/bin/bash
# Perf profiler

if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <results_dir> <executable> [executable args...]"
  exit 1
fi

RESULTS_DIR="$1"
EXECUTABLE="$2"
shift 2  # Shift away the first two parameters, leaving only executable args

# Ensure the output directory exists
mkdir -p "$RESULTS_DIR"

perf record -F 99 --call-graph dwarf -o "$RESULTS_DIR/perf.data" -- "$EXECUTABLE" "$@"
perf report -i "$RESULTS_DIR/perf.data"