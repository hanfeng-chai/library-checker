#pragma once
#include <toy/buffer.h>
namespace toy {
// CSR with a caller-chosen arc payload. Input records begin with (from,to).
template <class Arc>
struct Graph {
    Buffer<u32> offset;
    Buffer<Arc> edge;
    template <class E, class F>
    Graph(u32 n, std::span<const E> input, bool directed, F arc)
        : offset(n + 1), edge(input.size() * (directed ? 1 : 2)) {
        std::fill(offset.p, offset.p + n + 1, 0u);
        for (auto &e : input) {
            ++offset[e[0] + 1];
            if (!directed) ++offset[e[1] + 1];
        }
        for (u32 i = 1; i <= n; ++i) offset[i] += offset[i - 1];
        Buffer<u32> next(n);
        if (n) memcpy(next.p, offset.p, n * 4);
        for (u32 i = 0; i < input.size(); ++i) {
            auto &e = input[i];
            edge[next[e[0]]++] = arc(e[1], i);
            if (!directed) edge[next[e[1]]++] = arc(e[0], i);
        }
    }
    u32 size() const { return offset.n - 1; }
    std::span<const Arc> operator[](u32 v) const {
        return {edge.p + offset[v], offset[v + 1] - offset[v]};
    }
};
struct IndexedArc {
    u32 to, id;
};
struct WeightedArc {
    u32 to, weight;
};
inline auto indexed_graph(u32 n, std::span<const std::array<u32, 2>> edges, bool directed = false) {
    return Graph<IndexedArc>(n, edges, directed, [](u32 to, u32 id) { return IndexedArc{to, id}; });
}
inline auto weighted_graph(u32 n, std::span<const std::array<u32, 3>> edges, bool directed = true) {
    return Graph<WeightedArc>(n, edges, directed,
                              [&](u32 to, u32 id) { return WeightedArc{to, edges[id][2]}; });
}
} // namespace toy
