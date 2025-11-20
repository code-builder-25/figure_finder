#ifndef FIGURE_FINDER_FIGURE_FINDER_HPP
#define FIGURE_FINDER_FIGURE_FINDER_HPP

#include "matrix/matrix.hpp"
#include <cstdint>

namespace figures {

/**
 * This class defines an interface for figure detection algorithms.
 * Figures are connected components in a matrix where 1s represent
 * elements that may be part of a figure, and 0s represent empty space.
 * Figures are defined by cell side adjacency (up, down, left, right).
 */
class IFigureFinder {
public:
  virtual ~IFigureFinder() = default;

  // Count the number of distinct figures in a matrix.
  virtual size_t countFigures(const BoolMatrix& matrix) = 0;
};

} // namespace figures

#endif // FIGURE_FINDER_IFIGURE_FINDER_HPP
