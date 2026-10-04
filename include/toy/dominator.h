#pragma once
#include <toy/adjacency.h>
namespace toy {
// Lengauer-Tarjan. Unreachable vertices have idom = -1; idom[root] = root.
inline Buffer<i32> dominator_tree(u32 n, std::span<const std::array<u32, 2>> edges, u32 root) {
    Adjacency g(n, edges, true);
    Buffer<std::array<u32, 2>> reverse(edges.size());
    for (u32 i = 0; i < edges.size(); ++i) reverse[i] = {edges[i][1], edges[i][0]};
    Adjacency back(n, reverse, true);
    reverse = {};
    Buffer<u32> number(n), vertex(n), parent(n), cursor(n), path(n);
    std::fill(number.p, number.p + n, ~0u);
    memcpy(cursor.p, g.offset.p, n * 4);
    u32 count = 1, depth = 1;
    number[root] = 0;
    vertex[0] = root;
    parent[0] = 0;
    path[0] = root;
    while (depth) {
        u32 v = path[depth - 1];
        if (cursor[v] == g.offset[v + 1]) {
            --depth;
            continue;
        }
        u32 w = g.edge[cursor[v]++];
        if (number[w] != ~0u) continue;
        parent[count] = number[v];
        number[w] = count;
        vertex[count++] = w;
        path[depth++] = w;
    }
    Buffer<u32> semi(count), dsu(count), label(count), dom(count), head(count), next(count);
    std::iota(semi.p, semi.p + count, 0u);
    std::iota(dsu.p, dsu.p + count, 0u);
    std::iota(label.p, label.p + count, 0u);
    std::fill(head.p, head.p + count, ~0u);
    auto eval = [&](u32 v) {
        u32 top = 0, x = v;
        while (dsu[x] != dsu[dsu[x]]) {
            path[top++] = x;
            x = dsu[x];
        }
        while (top) {
            x = path[--top];
            u32 p = dsu[x];
            if (semi[label[p]] < semi[label[x]]) label[x] = label[p];
            dsu[x] = dsu[p];
        }
        return label[v];
    };
    for (u32 i = count; i--;) {
        for (u32 v : back[vertex[i]])
            if (number[v] != ~0u) semi[i] = std::min(semi[i], semi[eval(number[v])]);
        if (i) {
            next[i] = head[semi[i]];
            head[semi[i]] = i;
        }
        for (u32 v = head[i]; v != ~0u; v = next[v]) {
            u32 u = eval(v);
            dom[v] = semi[u] == semi[v] ? semi[v] : u;
        }
        if (i) dsu[i] = parent[i];
    }
    Buffer<i32> result(n);
    std::fill(result.p, result.p + n, -1);
    result[root] = root;
    for (u32 i = 1; i < count; ++i) {
        if (dom[i] != semi[i]) dom[i] = dom[dom[i]];
        result[vertex[i]] = vertex[dom[i]];
    }
    return result;
}
} // namespace toy
