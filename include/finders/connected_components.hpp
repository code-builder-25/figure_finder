#ifndef FIGURE_FINDER_CONNECTED_COMPONENTS_HPP
#define FIGURE_FINDER_CONNECTED_COMPONENTS_HPP

#include <cstdint>
#include <vector>

namespace figures {

// Graph representation of nodes with parent-child relations
// where each child points to the index of its parent.
// Components (trees) are connected by updating parent links
// until the root is reached, and attaching one tree to another.
// Size is used as sentinel for non-figure cells.
// The root of each component points to itself.
class ConnectedComponents {
public:
  ConnectedComponents(size_t size);

  void addNode(size_t idx);
  void connect(size_t a, size_t b);
  size_t getCount();

private:
  size_t updateRoot(size_t idx);

  std::vector<size_t> parent_;
};

} // namespace figures

#endif // FIGURE_FINDER_CONNECTED_COMPONENTS_HPP
