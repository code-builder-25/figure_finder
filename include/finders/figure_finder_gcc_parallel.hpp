#ifndef FIGURE_FINDER_GCC_PARALLEL_HPP
#define FIGURE_FINDER_GCC_PARALLEL_HPP

#include "figure_finder.hpp"

namespace figures {

// Parallel implementation of GCC algorithm
class FigureFinderGCCParallel : public IFigureFinder {
public:
  size_t countFigures(const BoolMatrix& matrix) override;
};

} // namespace figures

#endif // FIGURE_FINDER_GCC_PARALLEL_HPP
