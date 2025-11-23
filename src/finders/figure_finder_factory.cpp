#include "finders/figure_finder_factory.hpp"
#include "finders/figure_finder_bfs.hpp"
#include "finders/figure_finder_gcc.hpp"
#include "finders/figure_finder_gcc_parallel.hpp"
#include <array>
#include <functional>
#include <stdexcept>
#include <tuple>

namespace figures {

namespace {
using CreatorFunction = std::function<std::unique_ptr<IFigureFinder>()>;

inline const std::array<std::tuple<FigureFinderType, const char*, CreatorFunction>,
                        static_cast<size_t>(FigureFinderType::Count)>
    factoryArray = {
        {{FigureFinderType::BFS, "BFS", []() { return std::make_unique<FigureFinderBFS>(); }},
         {FigureFinderType::GCC, "GCC", []() { return std::make_unique<FigureFinderGCC>(); }},
         {FigureFinderType::GCCParallel, "GCCParallel",
          []() { return std::make_unique<FigureFinderGCCParallel>(); }}}};

static_assert(factoryArray.size() == static_cast<size_t>(FigureFinderType::Count),
              "Factory array does not cover all FigureFinderType values");
} // namespace

std::vector<FigureFinderType> getAllFigureFinderTypes() {
  std::vector<FigureFinderType> types;
  types.reserve(factoryArray.size());
  for (const auto& [type, _, __] : factoryArray) {
    types.push_back(type);
  }
  return types;
}

std::string toString(FigureFinderType type) {
  for (const auto& [key, name, _] : factoryArray) {
    if (key == type) {
      return name;
    }
  }
  return "Unknown";
}

std::unique_ptr<IFigureFinder> createFigureFinder(FigureFinderType type) {
  for (auto& [key, _, creator] : factoryArray) {
    if (key == type)
      return creator();
  }
  throw std::invalid_argument("Invalid FigureFinderType specified: " +
                              std::to_string(static_cast<int>(type)));
}

} // namespace figures
