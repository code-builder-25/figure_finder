#ifndef FIGURE_FINDER_FACTORY_HPP
#define FIGURE_FINDER_FACTORY_HPP

#include "figure_finder.hpp"
#include <memory>
#include <vector>

namespace figures {

enum class FigureFinderType {
  BFS,
  GCC,
  GCCParallel,
  Count // Always keep this last
};

std::vector<FigureFinderType> getAllFigureFinderTypes();
std::string toString(FigureFinderType type);

// Factory method to create a specific IFigureFinder implementation
std::unique_ptr<IFigureFinder> createFigureFinder(FigureFinderType algorithm);

} // namespace figures

#endif // FIGURE_FINDER_FACTORY_HPP
