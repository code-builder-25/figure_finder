#include "matrix/matrix.hpp"

namespace figures {

template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols, T default_value)
    : rows_(rows), cols_(cols), data_(rows * cols, default_value) {}

template <typename T> Matrix<T>::Matrix(const std::vector<std::vector<T>>& vec) {
  rows_ = vec.size();
  cols_ = 0;
  for (const auto& row : vec) {
    cols_ = std::max(cols_, row.size());
  }
  data_.reserve(rows_ * cols_);
  for (const auto& row : vec) {
    data_.insert(data_.end(), row.begin(), row.end());
    data_.insert(data_.end(), cols_ - row.size(), T{});
  }
}

template <typename T> T& Matrix<T>::operator()(size_t row, size_t col) {
  return data_[row * cols_ + col];
}

template <typename T> const T& Matrix<T>::operator()(size_t row, size_t col) const {
  return data_[row * cols_ + col];
}

template <typename T> size_t Matrix<T>::rows() const { return rows_; }

template <typename T> size_t Matrix<T>::cols() const { return cols_; }

template class Matrix<uint8_t>;

} // namespace figures