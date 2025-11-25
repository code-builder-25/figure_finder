#!/bin/bash
# Analyze Heaptrack report

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <results_dir> [threshold_bytes]"
  exit 1
fi

RESULTS_DIR="$1"
THRESHOLD_MB="$2"

PEAK_LINE=$(grep "peak heap memory consumption:" "$RESULTS_DIR/heaptrack_report.txt")
# Extract value and unit (e.g., "1234K" or "1.2M")
PEAK_VALUE=$(echo "$PEAK_LINE" | awk -F': ' '{print $2}')
PEAK_NUM=$(echo "$PEAK_VALUE" | sed -E 's/^([0-9.]+)[[:space:]]*([KMGkmg]?)$/\1/')
PEAK_UNIT=$(echo "$PEAK_VALUE" | sed -E 's/^([0-9.]+)[[:space:]]*([KMGkmg]?)$/\2/')

# Convert to MB
case "$PEAK_UNIT" in
  K|k) PEAK_MB=$((PEAK_NUM / 1024)) ;;
  M|m) PEAK_MB=$PEAK_NUM ;;
  G|g) PEAK_MB=$((PEAK_NUM * 1024)) ;;
  *) PEAK_MB=$((PEAK_NUM / 1024)) ;; # Assume KB if no unit
esac

if [ -n "$THRESHOLD_MB" ]; then
  if [ "$(echo "$PEAK_MB > $THRESHOLD_MB" | bc)" -eq 1 ]; then
    echo "❌ Heap memory too high: $PEAK_MB MB (threshold: $THRESHOLD_MB MB)"
    exit 1
  else
    echo "✅ Peak heap memory OK: $PEAK_MB MB"
  fi
else
  echo "Peak heap memory usage: $PEAK_MB MB"
fi