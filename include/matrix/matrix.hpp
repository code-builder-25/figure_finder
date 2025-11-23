#ifndef FIGURE_FINDER_MATRIX_HPP
#define FIGURE_FINDER_MATRIX_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

namespace figures {

// Generic 2D matrix
template <typename T> class Matrix {
public:
  Matrix(size_t rows = 0, size_t cols = 0, T default_value = T{});
  Matrix(const std::vector<std::vector<T>>& vec);

  T& operator()(size_t row, size_t col);
  const T& operator()(size_t row, size_t col) const;

  size_t rows() const;
  size_t cols() const;

private:
  size_t rows_;
  size_t cols_;
  std::vector<T> data_;
};

// Matrix of boolean values (0/1) stored as bytes
using BoolMatrix = Matrix<uint8_t>;

} // namespace figures

#endif // FIGURE_FINDER_MATRIX_HPP