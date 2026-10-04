#pragma once
#include <toy/dsu.h>
#include <toy/graph.h>
#include <toy/heap.h>
#include <toy/radix_sort.h>
namespace toy {
struct SpanningTree {
    u64 cost = 0;
    Buffer<u32> edge;
};
// Boruvka keeps one cheapest edge per component and compacts the remaining
// edge list after every round. Mark choices first so unions scan edges linearly.
inline SpanningTree boruvka(u32 n, std::span<const std::array<u32, 3>> input) {
    struct Edge {
        u32 u, v, w, id;
    };
    Buffer<Edge> edges(0, input.size());
    for (u32 i = 0; i < input.size(); ++i) {
        auto e = input[i];
        if (e[0] != e[1]) edges.p[edges.n++] = {e[0], e[1], e[2], i};
    }
    DSU dsu(n);
    Buffer<u64> best(n);
    Buffer<u32> active(n);
    std::iota(active.p, active.p + n, 0u);
    Buffer<u8> chosen(edges.n);
    SpanningTree result{0, Buffer<u32>(0, n ? n - 1 : 0)};
    while (edges.n) {
        for (u32 v : std::span(active.p, active.n)) best[v] = ~0ull;
        memset(chosen.p, 0, edges.n);
        for (u32 i = 0; i < edges.n; ++i) {
            auto e = edges[i];
            u64 key = (u64(e.w) << 32) | i;
            best[e.u] = std::min(best[e.u], key);
            best[e.v] = std::min(best[e.v], key);
        }
        for (u32 v : std::span(active.p, active.n))
            if (best[v] != ~0ull) chosen[u32(best[v])] = 1;
        for (u32 i = 0; i < edges.n; ++i)
            if (chosen[i]) {
                auto e = edges[i];
                if (dsu.merge(e.u, e.v)) {
                    result.cost += e.w;
                    result.edge.p[result.edge.n++] = e.id;
                }
            }
        if (result.edge.n + 1 == n) break;
        u32 kept = 0;
        for (auto e : std::span(edges.p, edges.n)) {
            e.u = dsu.leader(e.u);
            e.v = dsu.leader(e.v);
            if (e.u != e.v) edges[kept++] = e;
        }
        edges.n = kept;
        kept = 0;
        for (u32 v : std::span(active.p, active.n))
            if (dsu.parent[v] < 0) active[kept++] = v;
        active.n = kept;
    }
    return result;
}
// Dense Prim scans a contiguous matrix. A packed entry retains the original
// edge ID; zero labels mark selected vertices, so relaxation needs no mask.
inline SpanningTree dense_prim(u32 n, std::span<const std::array<u32, 3>> input) {
    Buffer<u64> matrix(usize(n) * n), key(n);
    std::fill(matrix.p, matrix.p + matrix.n, ~0ull);
    std::fill(key.p, key.p + n, ~0ull);
    for (u32 i = 0; i < input.size(); ++i) {
        auto e = input[i];
        u64 x = (u64(e[2]) << 32) | (i + 1);
        auto &a = matrix[usize(e[0]) * n + e[1]];
        a = std::min(a, x);
        matrix[usize(e[1]) * n + e[0]] = a;
    }
    SpanningTree result{0, Buffer<u32>(0, n ? n - 1 : 0)};
    for (u32 step = 0; step < n; ++step) {
        u32 v = 0;
        if (step) {
            u64 best = ~0ull;
            for (u32 u = 0; u < n; ++u)
                if (key[u] && key[u] < best) {
                    best = key[u];
                    v = u;
                }
            result.cost += best >> 32;
            result.edge.p[result.edge.n++] = u32(best) - 1;
        }
        key[v] = 0;
        const u64 *row = matrix.p + usize(v) * n;
        for (u32 u = 0; u < n; ++u) key[u] = std::min(key[u], row[u]);
    }
    return result;
}
inline SpanningTree kruskal(u32 n, std::span<const std::array<u32, 3>> input) {
    struct Edge {
        u32 u, v, w, id;
    };
    Buffer<Edge> sorted(input.size());
    for (u32 i = 0; i < input.size(); ++i) sorted[i] = {input[i][0], input[i][1], input[i][2], i};
    radix_sort32(std::span(sorted.p, sorted.n), [](Edge e) { return e.w; });
    DSU dsu(n);
    SpanningTree result{0, Buffer<u32>(n ? n - 1 : 0)};
    result.edge.n = 0;
    for (auto e : std::span(sorted.p, sorted.n))
        if (dsu.merge(e.u, e.v)) {
            result.cost += e.w;
            result.edge.p[result.edge.n++] = e.id;
            if (result.edge.n + 1 == n) break;
        }
    return result;
}
inline SpanningTree prim(u32 n, std::span<const std::array<u32, 3>> input) {
    auto g = Graph<IndexedArc>(n, input, false, [](u32 to, u32 id) { return IndexedArc{to, id}; });
    Buffer<u8> visited(n);
    if (n) memset(visited.p, 0, n);
    Buffer<u32> best(n);
    std::fill(best.p, best.p + n, ~0u);
    BinaryHeap<u64, true> queue;
    queue.values.reserve(input.size());
    SpanningTree result{0, Buffer<u32>(n ? n - 1 : 0)};
    result.edge.n = 0;
    auto enter = [&](u32 v) {
        visited[v] = 1;
        for (auto e : g[v])
            if (!visited[e.to] && input[e.id][2] < best[e.to]) {
                best[e.to] = input[e.id][2];
                queue.push((u64(best[e.to]) << 32) | e.id);
            }
    };
    if (n) enter(0);
    while (!queue.empty() && result.edge.n + 1 < n) {
        u64 x = queue.pop();
        u32 id = x;
        auto e = input[id];
        u32 v = visited[e[0]] ? e[1] : e[0];
        if (visited[v]) continue;
        result.cost += e[2];
        result.edge.p[result.edge.n++] = id;
        enter(v);
    }
    return result;
}
} // namespace toy
