#ifndef FIGURE_FINDER_GCC_HPP
#define FIGURE_FINDER_GCC_HPP

#include "figure_finder.hpp"

namespace figures {

// This class implements a Graph Connected Components (GCC) algorithm for counting figures.
class FigureFinderGCC : public IFigureFinder {
public:
  size_t countFigures(const BoolMatrix& matrix) override;
};

} // namespace figures

#endif // FIGURE_FINDER_GCC_HPP
