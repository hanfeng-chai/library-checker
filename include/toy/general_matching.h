#pragma once
#include <toy/adjacency.h>
namespace toy {
// Edmonds' alternating forest. Outer-outer edges contract odd cycles;
// predecessor links expand them implicitly when an augmenting path is flipped.
struct GeneralMatching {
    Buffer<u32> mate;
    u32 size = 0;
    GeneralMatching(u32 n, std::span<const std::array<u32, 2>> input) : mate(n + 1) {
        Buffer<std::array<u32, 2>> edges(input.size());
        for (u32 i = 0; i < input.size(); ++i) edges[i] = {input[i][0] + 1, input[i][1] + 1};
        Adjacency g(n + 1, edges);
        edges = {};
        std::fill(mate.p, mate.p + n + 1, 0u);
        Buffer<u32> base(n + 1), parent(n + 1), mark(n + 1), queue(n);
        Buffer<i8> type(n + 1);
        std::fill(parent.p, parent.p + n + 1, 0u);
        std::fill(mark.p, mark.p + n + 1, 0u);
        u32 stamp = 0;
        for (u32 u = 1; u <= n; ++u)
            if (!mate[u])
                for (u32 v : g[u])
                    if (v != u && !mate[v]) {
                        mate[u] = v;
                        mate[v] = u;
                        ++size;
                        break;
                    }
        for (u32 root = 1; root <= n; ++root)
            if (!mate[root]) {
                std::iota(base.p, base.p + n + 1, 0u);
                std::fill(type.p, type.p + n + 1, -1);
                u32 head = 0, tail = 1;
                queue[0] = root;
                type[root] = 0;
                bool found = false;
                auto lca = [&](u32 u, u32 v) {
                    ++stamp;
                    for (;;) {
                        if (u) {
                            if (mark[u] == stamp) return u;
                            mark[u] = stamp;
                            u = base[parent[mate[u]]];
                        }
                        std::swap(u, v);
                    }
                };
                auto contract = [&](u32 u, u32 v, u32 common) {
                    while (base[u] != common) {
                        parent[u] = v;
                        v = mate[u];
                        if (type[v] == 1) {
                            type[v] = 0;
                            queue[tail++] = v;
                        }
                        base[u] = base[v] = common;
                        u = parent[v];
                    }
                };
                while (head < tail && !found) {
                    u32 u = queue[head++];
                    for (u32 v : g[u]) {
                        if (type[v] < 0) {
                            parent[v] = u;
                            type[v] = 1;
                            u32 next = mate[v];
                            if (!next) {
                                for (u32 x = v; x;) {
                                    u32 p = parent[x], old = mate[p];
                                    mate[x] = p;
                                    mate[p] = x;
                                    x = old;
                                }
                                found = true;
                                ++size;
                                break;
                            }
                            type[next] = 0;
                            queue[tail++] = next;
                        } else if (!type[v] && base[u] != base[v]) {
                            u32 common = lca(u, v);
                            contract(u, v, common);
                            contract(v, u, common);
                            for (u32 x = 1; x <= n; ++x) base[x] = base[base[x]];
                        }
                    }
                }
            }
    }
};
} // namespace toy
