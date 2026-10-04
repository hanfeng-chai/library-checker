#pragma once
#include <toy/dsu.h>
#include <toy/graph.h>
#include <toy/heavy_light.h>
#include <toy/range_add_min.h>
namespace toy {
// The extreme sets of the original cut function form a laminar family.
// Minimum-adjacency contractions contain this family in a binary merge tree;
// changing an apex edge adds a constant along its leaf-to-root path.
struct ApexMinCut {
    HeavyLight hld;
    Buffer<i64> weight;
    std::optional<RangeAddMin> tree;
    u32 root;
    ApexMinCut(std::span<const i64> initial, std::span<const std::array<u32, 3>> edges)
        : hld(2 * initial.size() - 1), weight(initial.size()), root(2 * initial.size() - 2) {
        u32 n = initial.size();
        memcpy(weight.p, initial.data(), n * 8);
        DSU dsu(n);
        Buffer<u32> active(n), where(n), label(n), heap(n), position(n);
        std::iota(active.p, active.p + n, 0u);
        std::iota(where.p, where.p + n, 0u);
        std::iota(label.p, label.p + n, 0u);
        Buffer<i64> base(2 * n - 1), sum(2 * n - 1), key(n), degree(n);
        std::fill(base.p, base.p + base.n, 0ll);
        std::fill(sum.p, sum.p + sum.n, 0ll);
        for (u32 v = 0; v < n; ++v) sum[v] = initial[v];
        for (auto e : edges)
            if (e[0] != e[1]) {
                base[e[0]] += e[2];
                base[e[1]] += e[2];
            }
        for (u32 step = 0; step + 1 < n; ++step) {
            Buffer<std::array<u32, 3>> current(0, edges.size());
            std::fill(degree.p, degree.p + n, 0ll);
            for (auto e : edges) {
                u32 u = dsu.leader(e[0]), v = dsu.leader(e[1]);
                if (u != v) {
                    current.p[current.n++] = {u, v, e[2]};
                    degree[u] += e[2];
                    degree[v] += e[2];
                }
            }
            auto g = weighted_graph(n, std::span<const std::array<u32, 3>>(current), false);
            current = {};
            u32 size = active.n;
            std::fill(position.p, position.p + n, ~0u);
            for (u32 i = 0; i < size; ++i) {
                u32 v = active[i];
                heap[i] = v;
                position[v] = i;
                key[v] = degree[v];
            }
            auto less = [&](u32 a, u32 b) { return key[a] != key[b] ? key[a] < key[b] : a < b; };
            auto down = [&](u32 at) {
                u32 v = heap[at];
                for (u32 child; (child = 2 * at + 1) < size;) {
                    if (child + 1 < size && less(heap[child + 1], heap[child])) ++child;
                    if (!less(heap[child], v)) break;
                    heap[at] = heap[child];
                    position[heap[at]] = at;
                    at = child;
                }
                heap[at] = v;
                position[v] = at;
            };
            for (u32 i = size / 2; i--;) down(i);
            u32 a = 0, b = 0;
            while (size) {
                u32 v = heap[0];
                a = b;
                b = v;
                u32 last = heap[--size];
                if (size) {
                    heap[0] = last;
                    down(0);
                }
                position[v] = ~0u;
                for (auto e : g[v])
                    if (position[e.to] != ~0u) {
                        key[e.to] -= e.weight;
                        u32 at = position[e.to];
                        while (at) {
                            u32 up = (at - 1) / 2;
                            if (!less(e.to, heap[up])) break;
                            heap[at] = heap[up];
                            position[heap[at]] = at;
                            at = up;
                        }
                        heap[at] = e.to;
                        position[e.to] = at;
                    }
            }
            u32 node = n + step;
            hld.add_edge(node, label[a]);
            hld.add_edge(node, label[b]);
            sum[node] = sum[label[a]] + sum[label[b]];
            i64 between = 0;
            for (auto e : g[a])
                if (e.to == b) between += e.weight;
            base[node] = degree[a] + degree[b] - 2 * between;
            dsu.merge(a, b);
            u32 representative = dsu.leader(a), gone = representative == a ? b : a;
            label[representative] = node;
            u32 at = where[gone], last = active[--active.n];
            active[at] = last;
            where[last] = at;
        }
        hld.build(root);
        Buffer<i64> values(2 * n - 1);
        for (u32 v = 0; v < values.n; ++v) values[hld.position[v]] = base[v] + sum[v];
        tree.emplace(std::span<const i64>(values));
    }
    i64 set(u32 v, i64 value) {
        i64 delta = value - weight[v];
        weight[v] = value;
        for (;;) {
            u32 h = hld.head[v];
            tree->add(hld.position[h], hld.position[v] + 1, delta);
            if (h == root) break;
            v = hld.parent[h];
        }
        return tree->root;
    }
};
} // namespace toy
