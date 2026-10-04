#pragma once
#include <toy/bipartite_matching.h>
#include <toy/graph.h>
namespace toy {
// Compress same-side vertices, complete to a regular multigraph, then split
// even degrees by Euler tours and remove a perfect matching for odd degrees.
template <bool Random = true>
struct BipartiteColoring {
    struct Edge {
        u32 u, v, id;
    };
    u32 degree = 0;
    Buffer<u32> color;
    u64 seed = 0x9e3779b97f4a7c15ull;
    u32 random(u32 limit) {
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;
        return (u64(u32(seed)) * limit) >> 32;
    }
    BipartiteColoring(u32 l, u32 r, std::span<const std::array<u32, 2>> input)
        : color(input.size()) {
        if (input.empty()) return;
        Buffer<u32> dl(l), dr(r), left(l), right(r);
        std::fill(dl.p, dl.p + l, 0u);
        std::fill(dr.p, dr.p + r, 0u);
        for (auto e : input) {
            ++dl[e[0]];
            ++dr[e[1]];
        }
        for (auto x : std::span(dl.p, dl.n)) degree = std::max(degree, x);
        for (auto x : std::span(dr.p, dr.n)) degree = std::max(degree, x);
        auto compress = [&](Buffer<u32> &d, Buffer<u32> &map) {
            u32 n = 1, sum = 0;
            for (u32 i = 0; i < d.n; ++i) {
                if (sum + d[i] > degree) {
                    ++n;
                    sum = 0;
                }
                sum += d[i];
                map[i] = n - 1;
            }
            return n;
        };
        u32 n = std::max(compress(dl, left), compress(dr, right));
        Buffer<Edge> edges(usize(n) * degree);
        dl = Buffer<u32>(n);
        dr = Buffer<u32>(n);
        std::fill(dl.p, dl.p + n, 0u);
        std::fill(dr.p, dr.p + n, 0u);
        u32 m = 0;
        for (auto e : input) {
            u32 u = left[e[0]], v = right[e[1]];
            edges[m] = {u, v, m};
            ++m;
            ++dl[u];
            ++dr[v];
        }
        for (u32 u = 0, v = 0; u < n; ++u)
            while (dl[u] < degree) {
                while (dr[v] == degree) ++v;
                edges[m++] = {u, v, ~0u};
                ++dl[u];
                ++dr[v];
            }
        solve(n, degree, std::move(edges), 0);
    }
    void solve(u32 n, u32 d, Buffer<Edge> edges, u32 first) {
        auto paint = [&](Edge e, u32 c) {
            if (e.id != ~0u) color[e.id] = c;
        };
        if (d == 1) {
            for (auto e : std::span(edges.p, edges.n)) paint(e, first);
            return;
        }
        if (d & 1) {
            Buffer<u32> mate(n);
            std::fill(mate.p, mate.p + n, ~0u);
            if constexpr (Random) {
                Buffer<std::array<u32, 2>> pairs(edges.n);
                for (u32 i = 0; i < edges.n; ++i) pairs[i] = {edges[i].u, edges[i].v};
                Adjacency g(n, pairs, true);
                pairs = {};
                Buffer<u32> order(n);
                std::iota(order.p, order.p + n, 0u);
                // In a regular bipartite graph, random alternating walks reach
                // a free right vertex in expected O(n log n) total steps.
                for (u32 i = 0; i < n; ++i) {
                    std::swap(order[i], order[i + random(n - i)]);
                    u32 u = order[i];
                    while (u != ~0u) {
                        u32 v = g.edge[g.offset[u] + random(d)];
                        u = std::exchange(mate[v], u);
                    }
                }
            } else {
                Buffer<std::array<u32, 2>> pairs(edges.n);
                for (u32 i = 0; i < edges.n; ++i) pairs[i] = {edges[i].u, edges[i].v};
                BipartiteMatching matching(n, n, std::move(pairs));
                mate = std::move(matching.right);
            }
            Buffer<Edge> rest(0, edges.n - n);
            for (auto e : std::span(edges.p, edges.n)) {
                if (mate[e.v] == e.u) {
                    paint(e, first + d - 1);
                    mate[e.v] = ~0u;
                } else
                    rest.p[rest.n++] = e;
            }
            edges = {};
            solve(n, d - 1, std::move(rest), first);
            return;
        }
        Buffer<u32> offset(2 * n + 1), cursor(2 * n);
        for (u32 v = 0; v <= 2 * n; ++v) offset[v] = v * d;
        memcpy(cursor.p, offset.p, 2 * n * 4);
        Buffer<u32> arcs(edges.n * 2);
        for (u32 i = 0; i < edges.n; ++i) {
            auto e = edges[i];
            arcs[cursor[e.u]++] = i;
            arcs[cursor[e.v + n]++] = i;
        }
        memcpy(cursor.p, offset.p, 2 * n * 4);
        Buffer<u8> used(edges.n);
        memset(used.p, 0, used.n);
        Buffer<u32> vertices(edges.n + 1), path(edges.n), tour(0, edges.n);
        for (u32 root = 0; root < 2 * n; ++root) {
            u32 depth = 1;
            vertices[0] = root;
            while (depth) {
                u32 v = vertices[depth - 1];
                auto &at = cursor[v];
                while (at < offset[v + 1] && used[arcs[at]]) ++at;
                if (at == offset[v + 1]) {
                    if (--depth) tour.p[tour.n++] = path[depth - 1];
                    continue;
                }
                u32 id = arcs[at++];
                used[id] = 1;
                auto e = edges[id];
                path[depth - 1] = id;
                vertices[depth++] = v ^ e.u ^ (e.v + n);
            }
        }
        Buffer<Edge> a(edges.n / 2), b(edges.n / 2);
        for (u32 i = 0; i < edges.n; ++i) (i & 1 ? b : a)[i / 2] = edges[tour[i]];
        edges = {};
        solve(n, d / 2, std::move(a), first);
        solve(n, d / 2, std::move(b), first + d / 2);
    }
};
} // namespace toy
