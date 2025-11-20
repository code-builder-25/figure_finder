#include "finders/figure_finder_bfs.hpp"

#include <queue>

namespace figures {

size_t FigureFinderBFS::countFigures(const BoolMatrix& matrix) {
  // Create a visited matrix to track which cells have been processed
  BoolMatrix visited(matrix.rows(), matrix.cols(), 0);
  size_t figure_count = 0;
  std::queue<std::pair<size_t, size_t>> bfs_queue;
  const std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  for (size_t row = 0; row < matrix.rows(); ++row) {
    for (size_t column = 0; column < matrix.cols(); ++column) {
      if (matrix(row, column) && !visited(row, column)) {

        figure_count++;
        bfs_queue.push({row, column});
        visited(row, column) = 1;

        while (!bfs_queue.empty()) {
          auto [current_row, current_column] = bfs_queue.front();
          bfs_queue.pop();

          for (const auto& [direction_x, direction_y] : directions) {
            if (current_row == 0 && direction_x == -1 ||
                current_row == matrix.rows() - 1 && direction_x == 1 ||
                current_column == 0 && direction_y == -1 ||
                current_column == matrix.cols() - 1 && direction_y == 1)
              continue;

            auto next_row = current_row + direction_x;
            auto next_column = current_column + direction_y;
            if (matrix(next_row, next_column) && !visited(next_row, next_column)) {
              bfs_queue.push({next_row, next_column});
              visited(next_row, next_column) = 1;
            }
          }
        }
      }
    }
  }

  return figure_count;
}

} // namespace figures
