#include "finders/connected_components.hpp"

namespace figures {

ConnectedComponents::ConnectedComponents(size_t size) : parent_(size, size) {}

void ConnectedComponents::addNode(size_t idx) { parent_[idx] = idx; }

void ConnectedComponents::connect(size_t idx1, size_t idx2) {
  // need to update roots before connect as components might not get connected
  size_t root1 = updateRoot(idx1);
  size_t root2 = updateRoot(idx2);
  if (root1 != root2) {
    parent_[root1] = root2; // Attach idx1's tree to idx2's tree
  }
}

size_t ConnectedComponents::getCount() {
  size_t count = 0;
  for (size_t idx = 0; idx < parent_.size(); ++idx) {
    if (parent_[idx] == parent_.size())
      continue;
    // need to update roots as not all components have been connected on initialization
    size_t root = updateRoot(idx);
    if (root == idx) {
      count++;
    }
  }
  return count;
}

size_t ConnectedComponents::updateRoot(size_t idx) {
  while (parent_[idx] != idx) {
    size_t next = parent_[idx];
    parent_[idx] = parent_[next];
    idx = next;
  }
  return idx;
}

} // namespace figures