#!/bin/bash

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <executable> [executable args...]"
  exit 1
fi

EXECUTABLE="$1"
shift 1  # Shift away the first parameter, leaving only executable args

cmake -S . -B build_asan -G Ninja \
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DENABLE_ASAN=ON -DENABLE_UBSAN=ON
cmake --build build_asan -j;

ASAN_OPTIONS="detect_leaks=1:strict_string_checks=1:halt_on_error=1:abort_on_error=1" \
UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1:abort_on_error=1" \
./build_asan/bin/"$EXECUTABLE" "$@"