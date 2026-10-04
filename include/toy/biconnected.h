#pragma once
#include <toy/graph.h>
#include <toy/vertex_groups.h>
namespace toy {
// Undirected multigraph without self-loops. The vertex stack keeps a cut vertex
// while each child block is popped, so cut vertices may occur in several groups.
struct BiconnectedComponents : VertexGroups {
    explicit BiconnectedComponents(const Graph<IndexedArc> &g)
        : VertexGroups(2 * g.size(), g.size()) {
        u32 n = g.size(), time = 0, top = 0, depth = 0;
        Buffer<u32> order(n), active(n);
        std::fill(order.p, order.p + n, 0u);
        struct Frame {
            u32 v, next, low, parent;
        };
        Buffer<Frame> path(n);
        auto enter = [&](u32 v, u32 edge) {
            order[v] = ++time;
            active[top++] = v;
            path[depth++] = {v, g.offset[v], time, edge};
        };
        for (u32 root = 0; root < n; ++root)
            if (!order[root]) {
                enter(root, ~0u);
                while (depth) {
                    auto &f = path[depth - 1];
                    u32 v = f.v;
                    if (f.next < g.offset[v + 1]) {
                        auto e = g.edge[f.next++];
                        if (e.id == f.parent) continue;
                        if (!order[e.to])
                            enter(e.to, e.id);
                        else
                            f.low = std::min(f.low, order[e.to]);
                        continue;
                    }
                    u32 low = f.low;
                    --depth;
                    if (depth) {
                        auto &p = path[depth - 1];
                        p.low = std::min(p.low, low);
                        if (low >= order[p.v]) {
                            u32 u;
                            do {
                                u = active[--top];
                                add(u);
                            } while (u != v);
                            add(p.v);
                            finish();
                        }
                    } else {
                        --top;
                        if (g.offset[v] == g.offset[v + 1]) {
                            add(v);
                            finish();
                        }
                    }
                }
            }
    }
};
} // namespace toy
