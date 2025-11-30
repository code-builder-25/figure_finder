#include "finders/figure_finder_gcc_divided_parallel.hpp"
#include "finders/connected_components.hpp"

#include <thread>

namespace figures {

size_t FigureFinderGCCDividedParallel::countFigures(const BoolMatrix& matrix) {
  size_t rows = matrix.rows();
  size_t cols = matrix.cols();

  if (rows == 0 || cols == 0)
    return 0;

  // Convert 2D coordinates to 1D index
  auto to_index = [cols](size_t row, size_t col) -> size_t { return row * cols + col; };

  unsigned int num_threads = std::thread::hardware_concurrency();
  constexpr unsigned int kDefaultNumThreads = 4;
  if (num_threads == 0)
    num_threads = kDefaultNumThreads;
  const size_t rows_per_thread = rows < num_threads ? 1 : (rows + num_threads - 1) / num_threads;

  std::vector<std::vector<size_t>> vertices(num_threads);
  std::vector<std::vector<std::pair<size_t, size_t>>> edges(num_threads);
  std::vector<std::thread> threads;
  threads.reserve(num_threads);
  for (unsigned int thread_idx = 0; thread_idx < num_threads; ++thread_idx) {
    size_t start_row = thread_idx * rows_per_thread;
    size_t end_row = std::min(start_row + rows_per_thread, rows);

    threads.emplace_back([&, thread_idx, start_row, end_row]() {
      for (size_t row = start_row; row < end_row; ++row) {
        for (size_t col = 0; col < cols; ++col) {
          if (matrix(row, col)) {
            size_t current_idx = to_index(row, col);
            vertices[thread_idx].push_back(current_idx);

            // Link vertex to left and top neighbors if they exist
            if (col > 0 && matrix(row, col - 1)) {
              edges[thread_idx].emplace_back(current_idx, to_index(row, col - 1));
            }
            if (row > 0 && matrix(row - 1, col)) {
              edges[thread_idx].emplace_back(current_idx, to_index(row - 1, col));
            }
          }
        }
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }

  // Merge vertices and edges found by each thread, and connect them in the ConnectedComponents
  // structure
  ConnectedComponents components(rows * cols);
  for (unsigned int thread_idx = 0; thread_idx < num_threads; ++thread_idx) {
    for (const auto& v : vertices[thread_idx]) {
      components.addNode(v);
    }
  }

  for (unsigned int thread_idx = 0; thread_idx < num_threads; ++thread_idx) {
    for (const auto& e : edges[thread_idx]) {
      components.connect(e.first, e.second);
    }
  }

  return components.getCount();
}

} // namespace figures
