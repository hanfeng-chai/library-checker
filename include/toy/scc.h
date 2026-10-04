#pragma once
#include <toy/adjacency.h>
#include <toy/graph.h>
namespace toy {
template <bool Bridges, class G>
struct DFSComponents {
    Buffer<u32> id, offset, vertex;
    u32 size() const { return offset.n - 1; }
    std::span<const u32> operator[](u32 c) const {
        return {vertex.p + offset[c], offset[c + 1] - offset[c]};
    }
    explicit DFSComponents(const G &g) : id(g.size()), offset(g.size() + 1), vertex(g.size()) {
        u32 n = g.size(), time = 0, top = 0, count = 0, used = 0;
        Buffer<u32> order(n), active(n);
        std::fill(order.p, order.p + n, 0u);
        offset[0] = 0;
        struct BaseFrame {
            u32 v, next, low;
        };
        struct BridgeFrame : BaseFrame {
            u32 parent;
        };
        using Frame = std::conditional_t<Bridges, BridgeFrame, BaseFrame>;
        Buffer<Frame> path(n);
        u32 depth = 0;
        auto enter = [&](u32 v, u32 e) {
            order[v] = ++time;
            active[top++] = v;
            auto &f = path[depth++];
            f.v = v;
            f.next = g.offset[v];
            f.low = time;
            if constexpr (Bridges) f.parent = e;
        };
        for (u32 root = 0; root < n; ++root)
            if (!order[root]) {
                enter(root, ~0u);
                while (depth) {
                    auto &f = path[depth - 1];
                    u32 v = f.v;
                    if (f.next < g.offset[v + 1]) {
                        auto e = g.edge[f.next++];
                        u32 u, id = ~0u;
                        if constexpr (Bridges) {
                            if (e.id == f.parent) continue;
                            u = e.to;
                            id = e.id;
                        } else
                            u = e;
                        if (!order[u])
                            enter(u, id);
                        else
                            f.low = std::min(f.low, order[u]);
                        continue;
                    }
                    u32 low = f.low;
                    --depth;
                    if (depth) path[depth - 1].low = std::min(path[depth - 1].low, low);
                    if (low == order[v]) {
                        for (;;) {
                            u32 u = active[--top];
                            id[u] = count;
                            order[u] = ~0u;
                            vertex[used++] = u;
                            if (u == v) break;
                        }
                        offset[++count] = used;
                    }
                }
            }
        // Tarjan pops sinks first. Reverse the flat groups to expose topological IDs.
        std::reverse(vertex.p, vertex.p + n);
        std::reverse(offset.p, offset.p + count + 1);
        for (u32 i = 0; i <= count; ++i) offset[i] = n - offset[i];
        for (u32 v = 0; v < n; ++v) id[v] = count - 1 - id[v];
        offset.n = count + 1;
    }
};
using StrongComponents = DFSComponents<false, Adjacency>;
using TwoEdgeComponents = DFSComponents<true, Graph<IndexedArc>>;
} // namespace toy
