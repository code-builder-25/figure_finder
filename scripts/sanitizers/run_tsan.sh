#!/bin/bash

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <executable> [executable args...]"
  exit 1
fi

EXECUTABLE="$1"
shift 1  # Shift away the first parameter, leaving only executable args

cmake -S . -B build_tsan -G Ninja \
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DENABLE_TSAN=ON
cmake --build build_tsan -j;

TSAN_OPTIONS="halt_on_error=1:report_signal_unsafe=0" \
./build_tsan/bin/"$EXECUTABLE" "$@"