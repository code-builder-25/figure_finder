#!/bin/bash
# Heaptrack profiler

if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <results_dir> <executable> [executable args...]"
  exit 1
fi

RESULTS_DIR="$1"
EXECUTABLE="$2"
shift 2  # Shift away the first two parameters, leaving only executable args

# Ensure the output directory exists
mkdir -p "$RESULTS_DIR"

# Run heaptrack with the provided executable and arguments
echo "Running heaptrack..."
heaptrack --output $RESULTS_DIR/heaptrack.data "$EXECUTABLE" "$@"

# Generate readable report
heaptrack_print "$RESULTS_DIR/heaptrack.data.zst" > "$RESULTS_DIR/heaptrack_report.txt"

echo "Results:"
echo "  Raw output:    $RESULTS_DIR/heaptrack.data.zst"
echo "  Text report:   $RESULTS_DIR/heaptrack_report.txt"