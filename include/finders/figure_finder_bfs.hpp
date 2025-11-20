#ifndef FIGURE_FINDER_FIGURE_FINDER_BFS_HPP
#define FIGURE_FINDER_FIGURE_FINDER_BFS_HPP

#include "figure_finder.hpp"
#include <cstdint>

namespace figures {

// This class implements a Breadth-First Search (BFS) algorithm for counting figures.
class FigureFinderBFS : public IFigureFinder {
public:
  size_t countFigures(const BoolMatrix& matrix) override;
};

} // namespace figures

#endif // FIGURE_FINDER_BFS_HPP
