cmake_minimum_required(VERSION 3.16)
project(figure_finder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(GTEST_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)

option(ENABLE_WARNINGS "Enable extra warnings" ON)
option(ENABLE_WERROR "Treat warnings as errors" OFF)
option(ENABLE_ASAN  "Enable AddressSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSan." OFF)
option(ENABLE_TSAN  "Enable ThreadSanitizer" OFF)
option(ENABLE_MSAN  "Enable MemorySanitizer" OFF)

# Guard against incompatible mixes
if(ENABLE_ASAN AND ENABLE_TSAN)
  message(FATAL_ERROR "ASan and TSan cannot be used together.")
endif()
if(ENABLE_MSAN AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(FATAL_ERROR "MSan requires Clang.")
endif()

# Sanitizer settings
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_library(project_sanitizers INTERFACE)
  set(_SAN_FLAGS "")
  if(ENABLE_ASAN)
    list(APPEND _SAN_FLAGS -fsanitize=address)
  endif()
  if(ENABLE_UBSAN)
    list(APPEND _SAN_FLAGS -fsanitize=undefined)
  endif()
  if(ENABLE_TSAN)
    list(APPEND _SAN_FLAGS -fsanitize=thread)
  endif()
  if(ENABLE_MSAN)
    list(APPEND _SAN_FLAGS -fsanitize=memory -fsanitize-memory-track-origins=2)
  endif()

  target_compile_options(project_sanitizers INTERFACE
    -fno-omit-frame-pointer -g ${_SAN_FLAGS})
  target_link_options(project_sanitizers INTERFACE ${_SAN_FLAGS})
endif()

if(ENABLE_WARNINGS)
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
if(ENABLE_WERROR)
  add_compile_options(-Werror)
endif()

file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_SOURCE_DIR}/tests/*.cpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/*.cpp"
)

add_library(figure_finder STATIC
  src/finders/connected_components.cpp
  src/finders/figure_finder_bfs.cpp  
  src/finders/figure_finder_factory.cpp
  src/finders/figure_finder_gcc.cpp
  src/finders/figure_finder_gcc_divided_parallel.cpp
  src/finders/figure_finder_gcc_parallel.cpp
  src/matrix/matrix.cpp
  src/matrix/matrix_io.cpp
)
target_include_directories(figure_finder PUBLIC include)
target_link_libraries(figure_finder PUBLIC pthread)

# Main executable
add_executable(figure_finder_app src/main.cpp)
target_link_libraries(figure_finder_app PRIVATE figure_finder)


include(FetchContent)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(test_figure_finder tests/finders/test_finders_common.cpp
                                  tests/finders/test_finders_parallel.cpp)
target_link_libraries(test_figure_finder PRIVATE figure_finder GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test_figure_finder)

add_executable(test_matrix tests/matrix/test_matrix.cpp
                           tests/matrix/test_matrix_io.cpp)
target_link_libraries(test_matrix PRIVATE figure_finder GTest::gtest_main)
gtest_discover_tests(test_matrix)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.4
)
FetchContent_MakeAvailable(benchmark)

add_executable(bench_with_memory benchmarks/bench_with_memory.cpp)
target_link_libraries(bench_with_memory PRIVATE figure_finder benchmark::benchmark)

# Compare all algorithms
add_executable(compare_all benchmarks/compare_all.cpp)
target_link_libraries(compare_all PRIVATE figure_finder)

set(PROJECT_LOCAL_TARGETS
  figure_finder
  figure_finder_app
  test_figure_finder
  test_matrix
  bench_with_memory
  compare_all
)

foreach(t ${PROJECT_LOCAL_TARGETS})
  if(TARGET ${t} AND TARGET project_sanitizers)
    target_link_libraries(${t} PRIVATE project_sanitizers)
  endif()
endforeach()

# Convenience targets
add_custom_target(run-tests
  COMMAND ctest --output-on-failure
  DEPENDS test_figure_finder test_matrix
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(run-compare-all
  COMMAND compare_all
  DEPENDS compare_all
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(
    clang-format
    COMMAND clang-format
            -i ${ALL_CXX_SOURCE_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-format on all source files"
)

add_custom_target(
  clang-tidy
  COMMAND clang-tidy
          -p ${CMAKE_BINARY_DIR}
          ${ALL_CXX_SOURCE_FILES}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Running clang-tidy on all source files"
)
