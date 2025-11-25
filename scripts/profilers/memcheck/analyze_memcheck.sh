#!/bin/bash
# Analyze Valgrind Memcheck output and check for errors

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <results_dir>"
  exit 1
fi

RESULTS_DIR="$1"
MEMCHECK_FILE="$RESULTS_DIR/memcheck.txt"

echo "==== Valgrind Memcheck Analysis ===="

# Error summary
ERRORS=$(grep "ERROR SUMMARY:" "$MEMCHECK_FILE" | awk -F: '{print $2}' | awk '{print $1}')
echo "Error summary: $ERRORS errors"

# Heap summary
grep -A4 "HEAP SUMMARY:" "$MEMCHECK_FILE"

# Leak check
if grep -q "All heap blocks were freed -- no leaks are possible" "$MEMCHECK_FILE"; then
  echo "✅ No memory leaks detected."
else
  echo "❌ Memory leaks detected!"
  exit 1
fi

# Threshold check (default 0)
THRESHOLD=0
if [ "$ERRORS" -gt "$THRESHOLD" ]; then
  echo "❌ ERROR: $ERRORS memory errors detected (threshold: $THRESHOLD)"
  exit 1
else
  echo "✅ No memory errors detected (threshold: $THRESHOLD)"
fi

echo "===================================="