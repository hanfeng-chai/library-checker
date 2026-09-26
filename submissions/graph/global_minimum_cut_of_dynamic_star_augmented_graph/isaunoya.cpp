#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

/// @complexity Time: O(n(n+m) log n + mn) preprocessing and
/// O(log^2 n) per update. Space: O(n+m).

namespace noya {

namespace dynamic_star_min_cut_internal {

using i64 = std::int64_t;
static constexpr i64 infinity = i64(1) << 60;

class range_add_min_tree {
  int size = 1;
  std::vector<i64> minimum, lazy;

  void apply(int node, i64 addition) {
    minimum[node] += addition;
    lazy[node] += addition;
  }

  void push(int node) {
    apply(2 * node + 1, lazy[node]);
    apply(2 * node + 2, lazy[node]);
    lazy[node] = 0;
  }

  void add(int query_left, int query_right, int node, int left, int right,
           i64 addition) {
    if (right <= query_left || query_right <= left) {
      return;
    }
    if (query_left <= left && right <= query_right) {
      apply(node, addition);
      return;
    }
    push(node);
    int middle = (left + right) / 2;
    add(query_left, query_right, 2 * node + 1, left, middle, addition);
    add(query_left, query_right, 2 * node + 2, middle, right, addition);
    minimum[node] =
        std::min(minimum[2 * node + 1], minimum[2 * node + 2]);
  }

public:
  explicit range_add_min_tree(int n) {
    while (size < n) {
      size *= 2;
    }
    minimum.assign(2 * size - 1, infinity);
    lazy.assign(2 * size - 1, 0);
  }

  void add(int left, int right, i64 addition) {
    add(left, right, 0, 0, size, addition);
  }

  i64 get(int index) const {
    int node = index + size - 1;
    i64 result = minimum[node];
    while (node > 0) {
      node = (node - 1) / 2;
      result += lazy[node];
    }
    return result;
  }

  void set(int index, i64 value) { add(index, index + 1, value - get(index)); }

  i64 all_min() const { return minimum[0]; }
};

struct weighted_neighbor {
  int vertex;
  i64 weight;
};

/// Build the pendant-pair contraction tree.  Each merge creates the parent of
/// its two contracted clusters, so original vertices remain the leaves.
inline std::vector<std::vector<int>>
build_contraction_tree(std::vector<std::vector<weighted_neighbor>> graph) {
  int n = int(graph.size());
  std::vector<std::vector<int>> tree(2 * n - 1);
  std::vector<bool> active(2 * n - 1, true);
  for (int phase = 0; phase < n - 1; phase++) {
    graph.push_back({});
    std::vector<int> order(n - phase);
    std::vector<bool> visited(n + phase);
    std::vector<i64> incident(n + phase);
    using heap_entry = std::pair<i64, int>;
    std::priority_queue<heap_entry> queue;
    for (int vertex = 0; vertex < n + phase; vertex++) {
      if (!active[vertex]) {
        continue;
      }
      for (auto edge : graph[vertex]) {
        incident[vertex] += edge.weight;
      }
      queue.push({-incident[vertex], vertex});
    }

    for (int position = 0; position < n - phase; position++) {
      while (true) {
        auto [negated_cost, vertex] = queue.top();
        queue.pop();
        if (visited[vertex]) {
          continue;
        }
        visited[vertex] = true;
        order[position] = vertex;
        for (auto edge : graph[vertex]) {
          if (!visited[edge.vertex]) {
            incident[edge.vertex] -= edge.weight;
            queue.push({-incident[edge.vertex], edge.vertex});
          }
        }
        break;
      }
    }

    int first = order[n - phase - 1];
    int second = order[n - phase - 2];
    int merged = n + phase;
    graph[first].clear();
    graph[second].clear();
    active[first] = active[second] = false;
    for (int vertex = 0; vertex < merged; vertex++) {
      if (!active[vertex]) {
        continue;
      }
      for (weighted_neighbor &edge : graph[vertex]) {
        if (edge.vertex == first || edge.vertex == second) {
          edge.vertex = merged;
          graph[merged].push_back({vertex, edge.weight});
        }
      }
    }
    tree[merged].push_back(first);
    tree[merged].push_back(second);
  }
  return tree;
}

} // namespace dynamic_star_min_cut_internal

/// @brief Maintain the global min-cut after changing edges of an added star.
/// Repeated minimum-adjacency pendant-pair contractions build a binary cluster
/// tree with an optimal cut among its rooted clusters for every nonnegative
/// star weighting.  A cluster's cut is its original-graph boundary plus the
/// star weights of its leaves.  Thus a leaf update adds one value to all its
/// ancestors; heavy-light path updates and a global-min segment tree maintain
/// the best cluster cut.
class dynamic_star_min_cut {
public:
  using i64 = std::int64_t;
  struct edge {
    int first;
    int second;
    i64 weight;
  };

private:
  int vertex_count;
  std::vector<i64> star_weight;
  std::vector<std::vector<int>> tree;
  std::vector<int> parent, depth, heavy, head, order;
  dynamic_star_min_cut_internal::range_add_min_tree minimum;

  int build_heavy(int vertex) {
    int subtree_size = 1;
    int largest_child = 0;
    for (int child : tree[vertex]) {
      parent[child] = vertex;
      depth[child] = depth[vertex] + 1;
      int child_size = build_heavy(child);
      subtree_size += child_size;
      if (child_size > largest_child) {
        largest_child = child_size;
        heavy[vertex] = child;
      }
    }
    return subtree_size;
  }

  void build_order(int root) {
    int next_index = 0;
    std::queue<int> queue;
    queue.push(root);
    while (!queue.empty()) {
      int chain_head = queue.front();
      queue.pop();
      for (int vertex = chain_head; vertex != -1; vertex = heavy[vertex]) {
        order[vertex] = next_index++;
        head[vertex] = chain_head;
        for (int child : tree[vertex]) {
          if (child != heavy[vertex]) {
            queue.push(child);
          }
        }
      }
    }
  }

  i64 add_to_ancestors(int vertex, i64 addition) {
    while (vertex >= 0) {
      minimum.add(order[head[vertex]], order[vertex] + 1, addition);
      vertex = parent[head[vertex]];
    }
    return minimum.all_min();
  }

public:
  dynamic_star_min_cut(int n, const std::vector<edge> &edges,
                       std::vector<i64> initial_star_weight)
      : vertex_count(n), star_weight(std::move(initial_star_weight)),
        tree(), parent(2 * n - 1, -1), depth(2 * n - 1),
        heavy(2 * n - 1, -1), head(2 * n - 1), order(2 * n - 1, -1),
        minimum(2 * n - 1) {
    assert(n >= 1 && int(star_weight.size()) == n);
    std::vector<std::vector<dynamic_star_min_cut_internal::weighted_neighbor>>
        graph(n);
    for (auto [first, second, weight] : edges) {
      assert(0 <= first && first < n && 0 <= second && second < n);
      assert(weight >= 0);
      if (weight != 0) {
        graph[first].push_back({second, weight});
        graph[second].push_back({first, weight});
      }
    }
    tree = dynamic_star_min_cut_internal::build_contraction_tree(graph);
    int root = 2 * n - 2;
    build_heavy(root);
    build_order(root);

    std::vector<i64> boundary(2 * n - 1);
    for (auto [first, second, weight] : edges) {
      if (weight == 0) {
        continue;
      }
      int left = first;
      int right = second;
      while (left != right) {
        if (depth[left] < depth[right]) {
          std::swap(left, right);
        }
        boundary[left] += weight;
        left = parent[left];
      }
    }
    for (int vertex = 0; vertex < 2 * n - 1; vertex++) {
      minimum.set(order[vertex], boundary[vertex]);
    }
    for (int vertex = 0; vertex < n; vertex++) {
      assert(star_weight[vertex] >= 0);
      add_to_ancestors(vertex, star_weight[vertex]);
    }
  }

  /// Change one star-edge weight and return the new global minimum cut.
  i64 set_star_weight(int vertex, i64 weight) {
    assert(0 <= vertex && vertex < vertex_count && weight >= 0);
    i64 result = add_to_ancestors(vertex, weight - star_weight[vertex]);
    star_weight[vertex] = weight;
    return result;
  }

  i64 global_min_cut() const { return minimum.all_min(); }
};

} // namespace noya

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int vertex_count, edge_count, query_count;
  std::cin >> vertex_count >> edge_count >> query_count;
  std::vector<std::int64_t> star_weight(vertex_count);
  for (auto &weight : star_weight) {
    std::cin >> weight;
  }
  std::vector<noya::dynamic_star_min_cut::edge> edges(edge_count);
  for (auto &[first, second, weight] : edges) {
    std::cin >> first >> second >> weight;
  }
  noya::dynamic_star_min_cut structure(vertex_count, edges, star_weight);
  while (query_count--) {
    int vertex;
    std::int64_t weight;
    std::cin >> vertex >> weight;
    std::cout << structure.set_star_weight(vertex, weight) << '\n';
  }
}
