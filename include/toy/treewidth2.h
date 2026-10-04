#pragma once
#include <toy/hash.h>
#include <toy/page_buffer.h>
namespace toy {
// Width-two elimination for simple undirected graphs. Leaves need only degree
// and neighbor XOR. The remaining core reuses half-edges during suppression.
struct Treewidth2 {
    static constexpr u32 None = (1u << 21) - 1;
    struct Bag {
        u32 first = ~0u, second = ~0u;
    };
    Buffer<Bag> bag;
    Buffer<u32> parent;
    bool exists = false;
    Treewidth2(u32 n, std::span<const std::array<u32, 2>> edges) : bag(n), parent(n) {
        Buffer<u32> degree(n), neighbors(n), order(n), rank(n);
        Buffer<u8> queued(n);
        std::fill(degree.p, degree.p + n, 0u);
        std::fill(neighbors.p, neighbors.p + n, 0u);
        std::fill(queued.p, queued.p + n, 0);
        std::fill(bag.p, bag.p + n, Bag{});
        for (auto [u, v] : edges) {
            ++degree[u];
            ++degree[v];
            neighbors[u] ^= v;
            neighbors[v] ^= u;
        }
        u32 begin = 0, end = 0;
        auto offer = [&](u32 v, u32 limit) {
            if (!queued[v] && degree[v] <= limit) {
                queued[v] = 1;
                order[end++] = v;
            }
        };
        for (u32 v = 0; v < n; ++v) offer(v, 1);
        while (begin < end) {
            u32 v = order[begin];
            rank[v] = begin++;
            if (degree[v]) {
                u32 u = neighbors[v];
                bag[v].first = u;
                neighbors[u] ^= v;
                --degree[u];
                offer(u, 1);
            }
            degree[v] = 0;
        }
        if (begin < n) {
            struct Arc {
                u64 word;
                Arc(u32 to, u32 next, u32 twin)
                    : word(to | (u64(next) << 21) | (u64(twin) << 42)) {}
                u32 to() const { return word & None; }
                u32 next() const { return (word >> 21) & None; }
                u32 twin() const { return word >> 42; }
                void target(u32 v) { word = (word & ~u64(None)) | v; }
                void link(u32 v) { word = (word & ~(u64(None) << 21)) | (u64(v) << 21); }
                void reverse(u32 v) { word = (word & ~(u64(None) << 42)) | (u64(v) << 42); }
            };
            auto arc = page_buffer<Arc>(2 * edges.size());
            Buffer<u32> head(n);
            std::fill(head.p, head.p + n, None);
            u32 used = 0, high_vertices = 0, high_edges = 0;
            for (u32 v = 0; v < n; ++v) high_vertices += degree[v] > 32;
            for (auto [u, v] : edges)
                if (degree[u] && degree[v]) {
                    arc[used] = {v, head[u], used + 1};
                    head[u] = used++;
                    arc[used] = {u, head[v], used - 1};
                    head[v] = used++;
                    high_edges += (degree[u] > 32 && degree[v] > 32);
                }
            std::optional<HashMap<u64, u8>> high;
            auto key = [](u32 a, u32 b) { return (u64(std::min(a, b)) << 32) | std::max(a, b); };
            if (high_vertices > 1) {
                high.emplace(std::min<u64>(u64(high_vertices) * (high_vertices - 1) / 2,
                                           high_edges + n - begin));
                for (auto [u, v] : edges)
                    if (degree[u] > 32 && degree[v] > 32) (*high)[key(u, v)] = 1;
            }
            auto first = [&](u32 v) {
                auto &e = head[v];
                while (e != None && arc[e].to() == None) e = arc[e].next();
                return e;
            };
            auto adjacent = [&](u32 u, u32 v) {
                if (degree[u] > degree[v]) std::swap(u, v);
                if (degree[u] <= 32) {
                    for (u32 e = first(u); e != None;) {
                        if (arc[e].to() == v) return true;
                        u32 next = arc[e].next();
                        while (next != None && arc[next].to() == None) next = arc[next].next();
                        arc[e].link(next);
                        e = next;
                    }
                    return false;
                }
                auto &present = (*high)[key(u, v)];
                return bool(std::exchange(present, 1));
            };
            for (u32 v = 0; v < n; ++v)
                if (degree[v]) offer(v, 2);
            while (begin < end) {
                u32 v = order[begin];
                rank[v] = begin++;
                if (degree[v] == 1) {
                    u32 e = first(v), u = arc[e].to();
                    bag[v].first = u;
                    arc[arc[e].twin()].target(None);
                    neighbors[u] ^= v;
                    --degree[u];
                    offer(u, 2);
                } else if (degree[v] == 2) {
                    u32 e = first(v), f = arc[e].next();
                    while (arc[f].to() == None) f = arc[f].next();
                    arc[e].link(f);
                    u32 a = arc[e].to(), b = arc[f].to();
                    bag[v] = {a, b};
                    bool duplicate = degree[a] <= 2   ? (degree[a] == 2 && neighbors[a] == (v ^ b))
                                     : degree[b] <= 2 ? (degree[b] == 2 && neighbors[b] == (v ^ a))
                                                      : adjacent(a, b);
                    u32 x = arc[e].twin(), y = arc[f].twin();
                    if (duplicate) {
                        arc[x].target(None);
                        arc[y].target(None);
                        neighbors[a] ^= v;
                        neighbors[b] ^= v;
                        --degree[a];
                        --degree[b];
                        offer(a, 2);
                        offer(b, 2);
                    } else {
                        arc[x].target(b);
                        arc[y].target(a);
                        arc[x].reverse(y);
                        arc[y].reverse(x);
                        neighbors[a] ^= v ^ b;
                        neighbors[b] ^= v ^ a;
                    }
                }
                degree[v] = 0;
                head[v] = None;
            }
        }
        if (begin != n) return;
        exists = true;
        u32 root = ~0u;
        for (u32 v = 0; v < n; ++v) {
            auto b = bag[v];
            if (b.first == ~0u) {
                parent[v] = root == ~0u ? v : root;
                root = v;
            } else
                parent[v] = b.second == ~0u || rank[b.first] < rank[b.second] ? b.first : b.second;
        }
    }
};
} // namespace toy
