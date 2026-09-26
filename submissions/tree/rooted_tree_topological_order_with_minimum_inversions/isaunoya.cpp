#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

/// @complexity Time: O(n log n). Space: O(n).

namespace noya {

template <class Weight> struct rooted_tree_minimum_inversion_order_result {
  Weight cost{};
  std::vector<int> order;
};

namespace internal {

class labeled_dsu {
  std::vector<int> parent_;
  std::vector<int> label_;

  int leader(int vertex) {
    int root = vertex;
    while (parent_[root] >= 0) {
      root = parent_[root];
    }
    while (vertex != root) {
      int next = parent_[vertex];
      parent_[vertex] = root;
      vertex = next;
    }
    return root;
  }

public:
  explicit labeled_dsu(int size) : parent_(size, -1), label_(size) {
    std::iota(label_.begin(), label_.end(), 0);
  }

  int label(int vertex) { return label_[leader(vertex)]; }

  void merge(int first, int second, int new_label) {
    first = leader(first);
    second = leader(second);
    assert(first != second);
    if (parent_[first] > parent_[second]) {
      std::swap(first, second);
    }
    parent_[first] += parent_[second];
    parent_[second] = first;
    label_[first] = new_label;
  }
};

} // namespace internal

/// @brief Find a parent-before-child order minimizing the weighted inversion
/// cost. For two independent blocks A and B, placing A first contributes
/// d(A)c(B), while placing B first contributes d(B)c(A); hence the better
/// block order is decreasing c/d. Sidney's decomposition for an out-tree is
/// obtained by repeatedly contracting the maximum-ratio non-root block into
/// its parent block. A labeled DSU finds that current parent block, while a
/// cyclic linked list records the corresponding concatenations. The returned
/// cost is sum over i < j of d[order[i]] * c[order[j]]. All weights must be
/// nonnegative, and Weight must hold aggregate sums and their products.
template <class Weight>
rooted_tree_minimum_inversion_order_result<Weight>
rooted_tree_minimum_inversion_order(const std::vector<int> &parent,
                                    const std::vector<Weight> &c,
                                    const std::vector<Weight> &d) {
  int size = int(parent.size());
  assert(size > 0);
  assert(int(c.size()) == size && int(d.size()) == size);
  int root = -1;
  for (int vertex = 0; vertex < size; vertex++) {
    assert(c[vertex] >= Weight{} && d[vertex] >= Weight{});
    if (parent[vertex] == -1) {
      assert(root == -1);
      root = vertex;
    } else {
      assert(0 <= parent[vertex] && parent[vertex] < size);
      assert(parent[vertex] != vertex);
    }
  }
  assert(root != -1);

  struct block {
    Weight d;
    Weight c;
    int root;
    int version;
  };
  struct lower_priority {
    static bool ratio_less(const block &left, const block &right) {
      bool left_zero = left.c == Weight{} && left.d == Weight{};
      bool right_zero = right.c == Weight{} && right.d == Weight{};
      if (left_zero != right_zero) {
        return left_zero;
      }
      if (left_zero) {
        return false;
      }
      return left.c * right.d < left.d * right.c;
    }

    bool operator()(const block &left, const block &right) const {
      if (ratio_less(left, right)) {
        return true;
      }
      if (ratio_less(right, left)) {
        return false;
      }
      return std::tie(left.root, left.version) <
             std::tie(right.root, right.version);
    }
  };

  std::vector<Weight> block_c = c;
  std::vector<Weight> block_d = d;
  std::vector<int> version(size);
  std::priority_queue<block, std::vector<block>, lower_priority> queue;
  for (int vertex = 0; vertex < size; vertex++) {
    if (vertex != root) {
      queue.push({block_d[vertex], block_c[vertex], vertex, 0});
    }
  }

  internal::labeled_dsu components(size);
  std::vector<int> next(size);
  std::iota(next.begin(), next.end(), 0);
  while (!queue.empty()) {
    block current = queue.top();
    queue.pop();
    int vertex = current.root;
    if (current.version != version[vertex]) {
      continue;
    }
    int parent_block = components.label(parent[vertex]);
    block_c[parent_block] += block_c[vertex];
    block_d[parent_block] += block_d[vertex];
    components.merge(vertex, parent_block, parent_block);
    if (parent_block != root) {
      int new_version = ++version[parent_block];
      queue.push({block_d[parent_block], block_c[parent_block], parent_block,
                  new_version});
    }
    std::swap(next[vertex], next[parent_block]);
  }

  rooted_tree_minimum_inversion_order_result<Weight> result;
  result.order.reserve(size);
  int vertex = root;
  for (int index = 0; index < size; index++) {
    vertex = next[vertex];
    result.order.push_back(vertex);
  }
  assert(vertex == root);
  std::reverse(result.order.begin(), result.order.end());

  Weight prefix_d{};
  for (int current : result.order) {
    result.cost += prefix_d * c[current];
    prefix_d += d[current];
  }
  return result;
}

} // namespace noya

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n;
  std::cin >> n;
  std::vector<int> parent(n, -1);
  for (int vertex = 1; vertex < n; vertex++) {
    std::cin >> parent[vertex];
  }
  std::vector<std::int64_t> c(n), d(n);
  for (auto &value : c) {
    std::cin >> value;
  }
  for (auto &value : d) {
    std::cin >> value;
  }

  auto result = noya::rooted_tree_minimum_inversion_order(parent, c, d);
  std::cout << result.cost << '\n';
  for (int index = 0; index < n; index++) {
    std::cout << result.order[index] << " \n"[index + 1 == n];
  }
}
