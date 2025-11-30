#ifndef FIGURE_FINDER_GCC_DIVIDED_PARALLEL_HPP
#define FIGURE_FINDER_GCC_DIVIDED_PARALLEL_HPP

#include "figure_finder.hpp"

namespace figures {

// Parallel implementation of GCC algorithm which first creates
// parent-child relationships on a divided graph
// and then merges them and creates connected components.
class FigureFinderGCCDividedParallel : public IFigureFinder {
public:
  size_t countFigures(const BoolMatrix& matrix) override;
};

} // namespace figures

#endif // FIGURE_FINDER_GCC_DIVIDED_PARALLEL_HPP
