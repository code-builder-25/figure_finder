#!/bin/bash
# Heap growth over time profiler using Valgrind Massif

if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <results_dir> <executable> [executable args...]"
  exit 1
fi

RESULTS_DIR="$1"
EXECUTABLE="$2"
shift 2  # Shift away the first two parameters, leaving only executable args

# Ensure the output directory exists
mkdir -p "$RESULTS_DIR"

# Run valgrind massif with the provided executable and arguments
echo "Running Valgrind Massif..."
valgrind --tool=massif \
  --massif-out-file="$RESULTS_DIR/massif.out" \
  --pages-as-heap=yes \
  "$EXECUTABLE" "$@"

# Generate readable report
ms_print "$RESULTS_DIR/massif.out" > "$RESULTS_DIR/massif_report.txt"

echo "Results:"
echo "  Raw output:    $RESULTS_DIR/massif.out"
echo "  Text report:   $RESULTS_DIR/massif_report.txt"