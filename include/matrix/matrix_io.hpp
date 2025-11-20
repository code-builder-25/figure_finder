#ifndef FIGURE_FINDER_MATRIX_IO_HPP
#define FIGURE_FINDER_MATRIX_IO_HPP

#include "matrix/matrix.hpp"
#include <string>

namespace figures {

template <typename T> Matrix<T> loadMatrixFromFile(const std::string& filename);

} // namespace figures

#endif // FIGURE_FINDER_MATRIX_IO_HPP
