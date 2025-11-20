#include "finders/figure_finder_gcc.hpp"

#include <vector>

namespace figures {

namespace {

// Graph representation of parent-child relations
// where each child points to the index of its parent.
// Components (trees) are connected by updating parent links.
// Size is used as sentinel for non-figure cells.
// The root of each component points to itself.
class ConnectedComponents {
public:
  ConnectedComponents(size_t size) : parent_(size, size) {}
  size_t getCount();
  void addNode(size_t idx);
  size_t updateRoot(size_t idx);
  void connect(size_t a, size_t b);

private:
  std::vector<size_t> parent_;
};

size_t ConnectedComponents::getCount() {
  size_t count = 0;
  for (size_t idx = 0; idx < parent_.size(); ++idx) {
    if (parent_[idx] == parent_.size())
      continue;
    // need to update roots as not all components have been connected on initialization
    size_t root = updateRoot(idx);
    if (root == idx) {
      count++;
    }
  }
  return count;
}

void ConnectedComponents::addNode(size_t idx) { parent_[idx] = idx; }

size_t ConnectedComponents::updateRoot(size_t idx) {
  size_t root = idx;
  while (parent_[root] != root) {
    size_t next = parent_[root];
    parent_[root] = parent_[next];
    root = next;
  }
  return root;
}

void ConnectedComponents::connect(size_t a, size_t b) {
  size_t root_a = updateRoot(a);
  size_t root_b = updateRoot(b);
  // need to update roots before connect as components might not get connected
  if (root_a != root_b) {
    parent_[root_a] = root_b; // Attach a's tree to b's tree
  }
}

} // namespace

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
