#pragma once
#include <toy/adjacency.h>
#include <toy/graph.h>
#include <toy/radix_sort.h>
namespace toy {
// Number of four-cycles containing each original edge of a loopless multigraph.
// Parallel edges are compressed into weights before degree ordering.
template <bool Shrink = true>
Buffer<u64> count_c4(u32 n, std::span<const std::array<u32, 2>> input) {
    struct Record {
        u64 key;
        u32 id;
    };
    Buffer<Record> sorted(input.size());
    for (u32 i = 0; i < input.size(); ++i) {
        auto e = input[i];
        if (e[0] > e[1]) std::swap(e[0], e[1]);
        sorted[i] = {(u64(e[0]) << 32) | e[1], i};
    }
    radix_sort<64, 11>(std::span(sorted.p, sorted.n), [](Record x) { return x.key; });
    Buffer<std::array<u32, 2>> edges(0, input.size());
    Buffer<u32> weight(input.size()), original(input.size()), degree(n);
    std::fill(degree.p, degree.p + n, 0u);
    u64 last = ~0ull;
    for (auto x : std::span(sorted.p, sorted.n)) {
        if (x.key != last) {
            u32 u = x.key >> 32, v = x.key;
            edges.p[edges.n] = {u, v};
            weight[edges.n++] = 0;
            ++degree[u];
            ++degree[v];
            last = x.key;
        }
        original[x.id] = edges.n - 1;
        ++weight[edges.n - 1];
    }
    sorted = {};
    if constexpr (Shrink)
        if (edges.n == input.size() && n <= 4096 && edges.n > u64(n) * n / 8) {
            // For dense simple graphs, compute A^2 with bit intersections. The
            // three-step walk count subtracts exactly the walks repeating u or v.
            Adjacency g(n, edges);
            u32 words = (n + 63) / 64;
            Buffer<u64> bits(usize(n) * words);
            std::fill(bits.p, bits.p + bits.n, 0ull);
            for (auto e : input) {
                bits[usize(e[0]) * words + e[1] / 64] |= 1ull << (e[1] % 64);
                bits[usize(e[1]) * words + e[0] / 64] |= 1ull << (e[0] % 64);
            }
            Buffer<u32> common(usize(n) * n);
            for (u32 u = 0; u < n; ++u) {
                common[usize(u) * n + u] = degree[u];
                for (u32 v = 0; v < u; ++v) {
                    u32 count = 0;
                    for (u32 j = 0; j < words; ++j)
                        count +=
                            std::popcount(bits[usize(u) * words + j] & bits[usize(v) * words + j]);
                    common[usize(u) * n + v] = common[usize(v) * n + u] = count;
                }
            }
            Buffer<u64> result(input.size());
            for (u32 i = 0; i < input.size(); ++i) {
                auto e = input[i];
                u32 u = e[0], v = e[1];
                if (degree[u] > degree[v]) std::swap(u, v);
                const u32 *row = common.p + usize(v) * n;
                u32 sum = 0;
                for (u32 x : g[u]) sum += row[x];
                result[i] = sum - degree[u] - degree[v] + 1;
            }
            return result;
        }
    Buffer<u64> order(n);
    for (u32 v = 0; v < n; ++v) order[v] = (u64(degree[v]) << 32) | v;
    radix_sort<32, 10>(std::span(order.p, order.n), [](u64 x) { return x >> 32; });
    Buffer<u32> rank(n);
    for (u32 v = 0; v < n; ++v) rank[u32(order[v])] = v;
    for (auto &e : std::span(edges.p, edges.n)) {
        e = {rank[e[0]], rank[e[1]]};
        if (e[0] < e[1]) std::swap(e[0], e[1]);
    }
    Buffer<u32> offset(n + 1), end(n);
    offset[0] = 0;
    for (u32 v = 0; v < n; ++v) offset[v + 1] = offset[v] + degree[u32(order[v])];
    memcpy(end.p, offset.p, n * 4);
    Buffer<IndexedArc> arc(edges.n * 2);
    for (u32 i = 0; i < edges.n; ++i) {
        auto e = edges[i];
        arc[end[e[0]]++] = {e[1], i};
    }
    // Append upward arcs in vertex order. Removing the current largest vertex
    // then consists of decrementing each neighbor's row end.
    for (u32 v = 0; v < n; ++v)
        for (u32 i = offset[v]; i < end[v]; ++i) {
            auto e = arc[i];
            if (e.to < v) arc[end[e.to]++] = {v, e.id};
        }
    Buffer<u64> count(n), answer(edges.n), result(input.size());
    std::fill(count.p, count.p + n, 0ull);
    std::fill(answer.p, answer.p + answer.n, 0ull);
    for (u32 v = n; v--;) {
        if constexpr (Shrink)
            for (u32 i = offset[v]; i < end[v]; ++i) --end[arc[i].to];
        auto paths = [&](auto action) {
            for (u32 i = offset[v]; i < end[v]; ++i) {
                auto a = arc[i];
                if constexpr (!Shrink)
                    if (a.to >= v) continue;
                for (u32 j = offset[a.to]; j < end[a.to]; ++j) {
                    auto b = arc[j];
                    if constexpr (!Shrink)
                        if (b.to >= v) continue;
                    action(a, b);
                }
            }
        };
        paths([&](auto a, auto b) { count[b.to] += u64(weight[a.id]) * weight[b.id]; });
        paths([&](auto a, auto b) {
            u64 others = count[b.to] - u64(weight[a.id]) * weight[b.id];
            answer[a.id] += others * weight[b.id];
            answer[b.id] += others * weight[a.id];
        });
        paths([&](auto, auto b) { count[b.to] = 0; });
    }
    for (u32 i = 0; i < input.size(); ++i) result[i] = answer[original[i]];
    return result;
}
} // namespace toy
