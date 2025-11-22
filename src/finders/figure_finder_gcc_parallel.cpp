#include "finders/figure_finder_gcc_parallel.hpp"
#include "finders/connected_components.hpp"

#include <thread>

namespace figures {

size_t FigureFinderGCCParallel::countFigures(const BoolMatrix& matrix) {
  size_t rows = matrix.rows();
  size_t cols = matrix.cols();

  if (rows == 0 || cols == 0)
    return 0;

  // Convert 2D coordinates to 1D index
  auto to_index = [cols](size_t row, size_t col) -> size_t { return row * cols + col; };
  ConnectedComponents components(rows * cols);

  unsigned int num_threads = std::thread::hardware_concurrency();
  constexpr unsigned int kDefaultNumThreads = 4;
  if (num_threads == 0)
    num_threads = kDefaultNumThreads;
  const size_t rows_per_thread = rows < num_threads ? 1 : (rows + num_threads - 1) / num_threads;

  std::vector<std::thread> threads;
  threads.reserve(num_threads);
  for (unsigned int thread_idx = 0; thread_idx < num_threads; ++thread_idx) {
    size_t start_row = thread_idx * rows_per_thread;
    size_t end_row = std::min(start_row + rows_per_thread, rows);

    threads.emplace_back([&, start_row, end_row]() {
      for (size_t row = start_row; row < end_row; ++row) {
        for (size_t col = 0; col < cols; ++col) {
          if (matrix(row, col)) {
            size_t current_idx = to_index(row, col);
            components.addNode(current_idx);

            // Connect to left and top neighbors if they exist
            if (col > 0 && matrix(row, col - 1)) {
              components.connect(current_idx, to_index(row, col - 1));
            }
            if (row > start_row && matrix(row - 1, col)) {
              components.connect(current_idx, to_index(row - 1, col));
            }
          }
        }
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }

  // Loop over the border rows to connect components across thread's work boundaries
  for (unsigned int thread_idx = 1; thread_idx < num_threads; ++thread_idx) {
    size_t start_row = thread_idx * rows_per_thread;
    size_t end_row = std::min(start_row + 1, rows);

    for (size_t row = start_row; row < end_row; ++row) {
      for (size_t col = 0; col < cols; ++col) {
        if (matrix(row, col) && matrix(row - 1, col)) {
          // Connect to top neighbor
          components.connect(to_index(row, col), to_index(row - 1, col));
        }
      }
    }
  }

  return components.getCount();
}

} // namespace figures
