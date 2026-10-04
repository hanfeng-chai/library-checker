#pragma once
#include <toy/graph.h>
namespace toy {
struct SteinerTree {
    u64 cost;
    Buffer<u32> edge;
};
inline SteinerTree steiner_tree(u32 n, std::span<const std::array<u32, 3>> edges,
                                std::span<const u32> terminals) {
    constexpr u64 Infinity = 1ull << 60;
    u32 bits = terminals.size() - 1, states = 1u << bits;
    Buffer<u64> distance(usize(n) * n);
    std::fill(distance.p, distance.p + distance.n, Infinity);
    for (u32 i = 0; i < n; ++i) distance[usize(i) * n + i] = 0;
    for (auto e : edges) {
        auto &x = distance[usize(e[0]) * n + e[1]];
        x = std::min(x, u64(e[2]));
        distance[usize(e[1]) * n + e[0]] = x;
    }
    for (u32 k = 0; k < n; ++k)
        for (u32 i = 0; i < n; ++i) {
            u64 d = distance[usize(i) * n + k];
            auto row = distance.p + usize(i) * n, other = distance.p + usize(k) * n;
            for (u32 j = 0; j < n; ++j) row[j] = std::min(row[j], d + other[j]);
        }
    Buffer<u64> dp(usize(states) * n), merged(n);
    std::fill(dp.p, dp.p + dp.n, Infinity);
    std::fill(dp.p, dp.p + n, 0ull);
    for (u32 i = 0; i < bits; ++i) dp[usize(1u << i) * n + terminals[i]] = 0;
    for (u32 mask = 1; mask < states; ++mask) {
        auto row = dp.p + usize(mask) * n;
        for (u32 sub = (mask - 1) & mask; sub; sub = (sub - 1) & mask) {
            u32 other = mask ^ sub;
            if (sub < other) continue;
            auto a = dp.p + usize(sub) * n, b = dp.p + usize(other) * n;
            for (u32 v = 0; v < n; ++v) row[v] = std::min(row[v], a[v] + b[v]);
        }
        memcpy(merged.p, row, n * 8);
        for (u32 u = 0; u < n; ++u)
            if (merged[u] < Infinity) {
                auto d = distance.p + usize(u) * n;
                u64 x = merged[u];
                for (u32 v = 0; v < n; ++v) row[v] = std::min(row[v], x + d[v]);
            }
    }
    SteinerTree result{dp[usize(states - 1) * n + terminals.back()], Buffer<u32>(0, edges.size())};
    Buffer<u8> used(edges.size());
    if (used.n) memset(used.p, 0, used.n);
    auto graph =
        Graph<IndexedArc>(n, edges, false, [](u32 to, u32 id) { return IndexedArc{to, id}; });
    auto trace = [&](auto &&self, u32 mask, u32 v) -> void {
        u64 want = dp[usize(mask) * n + v];
        if (!want) return;
        for (auto e : graph[v])
            if (dp[usize(mask) * n + e.to] + edges[e.id][2] == want) {
                if (!used[e.id]) {
                    used[e.id] = 1;
                    result.edge.p[result.edge.n++] = e.id;
                }
                self(self, mask, e.to);
                return;
            }
        for (u32 sub = (mask - 1) & mask; sub; sub = (sub - 1) & mask)
            if (dp[usize(sub) * n + v] + dp[usize(mask ^ sub) * n + v] == want) {
                self(self, sub, v);
                self(self, mask ^ sub, v);
                return;
            }
    };
    trace(trace, states - 1, terminals.back());
    return result;
}
} // namespace toy
