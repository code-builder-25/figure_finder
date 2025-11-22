#include "finders/figure_finder_gcc.hpp"
#include "finders/connected_components.hpp"

namespace figures {

size_t FigureFinderGCC::countFigures(const BoolMatrix& matrix) {
  size_t rows = matrix.rows();
  size_t cols = matrix.cols();

  if (rows == 0 || cols == 0)
    return 0;

  // Convert 2D coordinates to 1D index
  auto to_index = [cols](size_t row, size_t col) -> size_t { return row * cols + col; };
  ConnectedComponents cc(rows * cols);

  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < cols; ++col) {
      if (matrix(row, col)) {
        size_t current_idx = to_index(row, col);
        cc.addNode(current_idx);

        // Connect to left and top neighbors if they exist
        if (col > 0 && matrix(row, col - 1)) {
          cc.connect(current_idx, to_index(row, col - 1));
        }
        if (row > 0 && matrix(row - 1, col)) {
          cc.connect(current_idx, to_index(row - 1, col));
        }
      }
    }
  }

  return cc.getCount();
}

} // namespace figures
