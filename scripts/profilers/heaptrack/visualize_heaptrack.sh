#!/bin/bash
# Visualizer for Heaptrack profiler reports

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <results_dir>"
  exit 1
fi

RESULTS_DIR="$1"

heaptrack_gui "$RESULTS_DIR/heaptrack.data.zst"