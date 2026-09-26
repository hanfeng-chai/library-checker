#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

/// @complexity Time: O(n + m sqrt(m)).
/// Space: O(n + m).

namespace noya {

namespace count_c4_detail {

inline std::vector<std::int64_t>
simple(int vertex_count, std::vector<int> first, std::vector<int> second,
       const std::vector<std::int64_t> &multiplicity) {
  int edge_count = int(multiplicity.size());
  std::vector<int> degree(vertex_count);
  for (int edge = 0; edge < edge_count; edge++) {
    degree[first[edge]]++;
    degree[second[edge]]++;
  }
  std::vector<int> order(vertex_count);
  std::iota(order.begin(), order.end(), 0);
  std::stable_sort(order.begin(), order.end(), [&](int left, int right) {
    return degree[left] < degree[right];
  });
  std::vector<int> rank(vertex_count);
  for (int i = 0; i < vertex_count; i++) {
    rank[order[i]] = i;
  }
  for (int &vertex : first) {
    vertex = rank[vertex];
  }
  for (int &vertex : second) {
    vertex = rank[vertex];
  }
  for (int edge = 0; edge < edge_count; edge++) {
    if (first[edge] < second[edge]) {
      std::swap(first[edge], second[edge]);
    }
  }

  std::vector<int> begin(vertex_count);
  for (int i = 0; i + 1 < vertex_count; i++) {
    begin[i + 1] = begin[i] + degree[order[i]];
  }
  std::vector<int> end = begin;
  std::vector<int> edge_id(edge_count * 2);
  std::vector<int> to(edge_count * 2);
  for (int edge = 0; edge < edge_count; edge++) {
    edge_id[end[first[edge]]] = edge;
    to[end[first[edge]]++] = second[edge];
  }
  std::vector<int> lower_end = end;
  for (int vertex = 0; vertex < vertex_count; vertex++) {
    for (int index = begin[vertex]; index < lower_end[vertex]; index++) {
      int edge = edge_id[index];
      int other = to[index];
      edge_id[end[other]] = edge;
      to[end[other]++] = vertex;
    }
  }

  std::vector<std::int64_t> path_count(vertex_count);
  std::vector<std::int64_t> answer(edge_count);
  for (int vertex = vertex_count - 1; vertex >= 0; vertex--) {
    for (int index = begin[vertex]; index < end[vertex]; index++) {
      int first_edge = edge_id[index];
      int middle = to[index];
      end[middle]--;
      for (int next = begin[middle]; next < end[middle]; next++) {
        int second_edge = edge_id[next];
        path_count[to[next]] +=
            multiplicity[first_edge] * multiplicity[second_edge];
      }
    }
    for (int index = begin[vertex]; index < end[vertex]; index++) {
      int first_edge = edge_id[index];
      int middle = to[index];
      for (int next = begin[middle]; next < end[middle]; next++) {
        int second_edge = edge_id[next];
        std::int64_t alternatives =
            path_count[to[next]] -
            multiplicity[first_edge] * multiplicity[second_edge];
        answer[first_edge] += alternatives * multiplicity[second_edge];
        answer[second_edge] += alternatives * multiplicity[first_edge];
      }
    }
    for (int index = begin[vertex]; index < end[vertex]; index++) {
      int middle = to[index];
      for (int next = begin[middle]; next < end[middle]; next++) {
        path_count[to[next]] = 0;
      }
    }
  }
  return answer;
}

} // namespace count_c4_detail

/// @brief Count, for every edge of an undirected multigraph, how many
/// four-edge subsets containing it form a simple four-cycle. Parallel edges
/// are first grouped into a weighted simple graph; the degree orientation then
/// charges every length-two path to a low-degree middle vertex.
inline std::vector<std::int64_t>
count_c4_per_edge(int vertex_count,
                  const std::vector<std::pair<int, int>> &edges) {
  int edge_count = int(edges.size());
  std::vector<int> order(edge_count);
  std::iota(order.begin(), order.end(), 0);
  std::vector<std::pair<int, int>> normalized = edges;
  for (auto &[first, second] : normalized) {
    if (first > second) {
      std::swap(first, second);
    }
  }
  std::stable_sort(order.begin(), order.end(), [&](int left, int right) {
    return normalized[left] < normalized[right];
  });

  std::vector<int> first, second, group(edge_count);
  std::vector<std::int64_t> multiplicity;
  for (int edge : order) {
    if (first.empty() || first.back() != normalized[edge].first ||
        second.back() != normalized[edge].second) {
      first.push_back(normalized[edge].first);
      second.push_back(normalized[edge].second);
      multiplicity.push_back(0);
    }
    multiplicity.back()++;
    group[edge] = int(first.size()) - 1;
  }
  auto simple_answer = count_c4_detail::simple(
      vertex_count, std::move(first), std::move(second), multiplicity);
  std::vector<std::int64_t> answer(edge_count);
  for (int edge = 0; edge < edge_count; edge++) {
    answer[edge] = simple_answer[group[edge]];
  }
  return answer;
}

} // namespace noya

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, m;
  std::cin >> n >> m;
  std::vector<std::pair<int, int>> edges(m);
  for (auto &[first, second] : edges) {
    std::cin >> first >> second;
  }
  auto answer = noya::count_c4_per_edge(n, edges);
  for (int edge = 0; edge < m; edge++) {
    std::cout << answer[edge] << " \n"[edge + 1 == m];
  }
}
