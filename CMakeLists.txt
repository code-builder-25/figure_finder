cmake_minimum_required(VERSION 3.16)
project(figure_finder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_WARNINGS "Enable extra warnings" ON)
option(ENABLE_WERROR "Treat warnings as errors" OFF)

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
)

add_library(figure_finder STATIC
  src/finders/connected_components.cpp
  src/finders/figure_finder_bfs.cpp  
  src/finders/figure_finder_factory.cpp
  src/finders/figure_finder_gcc.cpp
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

# Convenience targets
add_custom_target(run-tests
  COMMAND ctest --output-on-failure
  DEPENDS test_figure_finder test_matrix
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(
    clang-format
    COMMAND clang-format
            -ininj
            ${ALL_CXX_SOURCE_FILES}
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