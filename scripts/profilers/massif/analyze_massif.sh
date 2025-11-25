#!/bin/bash
# Analyze Valgrind Massif output

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <results_dir> [threshold_bytes]"
  exit 1
fi

RESULTS_DIR="$1"
THRESHOLD_MB="$2"

PEAK=$(grep "mem_heap_B=" $RESULTS_DIR/massif.out | awk -F= '{print $2}' | sort -n | tail -1)
PEAK_MB=$((PEAK / (1024 * 1024)))

if [ -n "$THRESHOLD_MB" ]; then
  if [ "$PEAK_MB" -gt "$THRESHOLD_MB" ]; then
    echo "❌ Memory too high: $PEAK_MB MB (threshold: $THRESHOLD_MB MB)"
    exit 1
  else
    echo "✅ Peak memory OK: $PEAK_MB MB"
  fi
else
  echo "Peak memory usage: $PEAK_MB MB"
fi