#include <bit>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

namespace noya {

/// @brief Return a maximum clique of an undirected graph with at most 64
/// vertices using branch-and-bound with greedy coloring.
inline std::vector<int>
maximum_clique(int n, const std::vector<std::pair<int, int>> &edges) {
  assert(0 <= n && n <= 64);
  std::vector<std::uint64_t> adjacency(n);
  for (auto [first, second] : edges) {
    assert(0 <= first && first < n);
    assert(0 <= second && second < n);
    if (first != second) {
      adjacency[first] |= std::uint64_t(1) << second;
      adjacency[second] |= std::uint64_t(1) << first;
    }
  }

  std::vector<int> best, current;
  auto expand = [&](auto &self, std::uint64_t candidates) -> void {
    if (candidates == 0) {
      if (current.size() > best.size()) {
        best = current;
      }
      return;
    }

    std::vector<int> order, color_bound;
    std::uint64_t remaining = candidates;
    for (int color = 1; remaining != 0; color++) {
      std::uint64_t available = remaining;
      while (available != 0) {
        int vertex = std::countr_zero(available);
        std::uint64_t bit = std::uint64_t(1) << vertex;
        remaining &= ~bit;
        available &= ~bit;
        order.push_back(vertex);
        color_bound.push_back(color);
        available &= ~adjacency[vertex];
        available &= remaining;
      }
    }

    for (int index = int(order.size()) - 1; index >= 0; index--) {
      if (current.size() + std::size_t(color_bound[index]) <= best.size()) {
        return;
      }
      int vertex = order[index];
      std::uint64_t bit = std::uint64_t(1) << vertex;
      if ((candidates & bit) == 0) {
        continue;
      }
      current.push_back(vertex);
      self(self, candidates & adjacency[vertex]);
      current.pop_back();
      candidates &= ~bit;
    }
  };

  std::uint64_t all =
      n == 64 ? ~std::uint64_t{} : ((std::uint64_t(1) << n) - 1);
  expand(expand, all);
  return best;
}

/// @brief Return a maximum independent set of an undirected graph with at most
/// 64 vertices by solving maximum clique on its complement.
inline std::vector<int>
maximum_independent_set(int n, const std::vector<std::pair<int, int>> &edges) {
  assert(0 <= n && n <= 64);
  std::vector<std::uint64_t> adjacency(n);
  for (auto [first, second] : edges) {
    assert(0 <= first && first < n);
    assert(0 <= second && second < n);
    if (first != second) {
      adjacency[first] |= std::uint64_t(1) << second;
      adjacency[second] |= std::uint64_t(1) << first;
    }
  }
  std::vector<std::pair<int, int>> complement;
  for (int first = 0; first < n; first++) {
    for (int second = first + 1; second < n; second++) {
      if (!(adjacency[first] >> second & 1)) {
        complement.emplace_back(first, second);
      }
    }
  }
  return maximum_clique(n, complement);
}

} // namespace noya

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, m;
  std::cin >> n >> m;
  std::vector<std::pair<int, int>> edges(m);
  for (auto &[u, v] : edges) std::cin >> u >> v;
  auto answer = noya::maximum_independent_set(n, edges);
  std::cout << answer.size() << '\n';
  for (int i = 0; i < int(answer.size()); ++i) std::cout << answer[i] << " \n"[i + 1 == int(answer.size())];
}
