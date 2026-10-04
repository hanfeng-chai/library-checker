#pragma once
#include <toy/graph.h>
namespace toy {
struct GraphCycle {
    Buffer<u32> vertex, edge;
};
// A gray DFS vertex stores its depth; ~0 denotes a finished vertex.
inline GraphCycle graph_cycle(const Graph<IndexedArc> &g, bool directed = true) {
    u32 n = g.size(), depth = 0;
    Buffer<u32> state(n);
    std::fill(state.p, state.p + n, 0u);
    struct Frame {
        u32 v, next, parent_edge;
    };
    Buffer<Frame> path(n);
    auto enter = [&](u32 v, u32 edge) {
        state[v] = depth + 1;
        path[depth++] = {v, g.offset[v], edge};
    };
    for (u32 root = 0; root < n; ++root)
        if (!state[root]) {
            enter(root, ~0u);
            while (depth) {
                auto &f = path[depth - 1];
                if (f.next == g.offset[f.v + 1]) {
                    state[f.v] = ~0u;
                    --depth;
                    continue;
                }
                auto e = g.edge[f.next++];
                if (!directed && e.id == f.parent_edge) continue;
                if (!state[e.to]) {
                    enter(e.to, e.id);
                    continue;
                }
                if (state[e.to] == ~0u) continue;
                u32 first = state[e.to] - 1, length = depth - first;
                GraphCycle result{Buffer<u32>(length), Buffer<u32>(length)};
                for (u32 i = 0; i < length; ++i) {
                    result.vertex[i] = path[first + i].v;
                    result.edge[i] = i + 1 < length ? path[first + i + 1].parent_edge : e.id;
                }
                return result;
            }
        }
    return {};
}
} // namespace toy
