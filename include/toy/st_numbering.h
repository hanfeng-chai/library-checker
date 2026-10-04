#pragma once
#include <toy/adjacency.h>
namespace toy {
// Returns vertex ranks, or an empty buffer when no s-t numbering exists.
inline Buffer<u32> st_numbering(const Adjacency &g, u32 s, u32 t) {
    u32 n = g.size();
    if (n == 1) {
        Buffer<u32> r(1);
        r[0] = 0;
        return r;
    }
    if (s == t) return {};
    Buffer<u32> pre(n), low(n), parent(n), order(n), cursor(n), path(n);
    std::fill(pre.p, pre.p + n, ~0u);
    memcpy(cursor.p, g.offset.p, n * 4);
    pre[s] = 0;
    low[s] = s;
    order[0] = s;
    pre[t] = 1;
    low[t] = t;
    order[1] = t;
    parent[t] = s;
    path[0] = t;
    u32 count = 2, depth = 1;
    while (depth) {
        u32 v = path[depth - 1];
        if (cursor[v] == g.offset[v + 1]) {
            --depth;
            if (depth) {
                u32 p = path[depth - 1];
                if (pre[low[v]] < pre[low[p]]) low[p] = low[v];
            }
            continue;
        }
        u32 w = g.edge[cursor[v]++];
        if (pre[w] == ~0u) {
            parent[w] = v;
            pre[w] = count;
            order[count++] = w;
            low[w] = w;
            path[depth++] = w;
        } else if (pre[w] < pre[low[v]])
            low[v] = w;
    }
    if (count != n) return {};
    Buffer<u32> next(n), previous(n);
    Buffer<i8> sign(n);
    std::fill(next.p, next.p + n, ~0u);
    std::fill(previous.p, previous.p + n, ~0u);
    std::fill(sign.p, sign.p + n, 0);
    next[s] = t;
    previous[t] = s;
    sign[s] = -1;
    for (u32 i = 2; i < n; ++i) {
        u32 v = order[i], p = parent[v];
        if (sign[low[v]] < 0) {
            u32 q = previous[p];
            if (q == ~0u) return {};
            next[q] = v;
            next[v] = p;
            previous[v] = q;
            previous[p] = v;
            sign[p] = 1;
        } else {
            u32 q = next[p];
            if (q == ~0u) return {};
            next[p] = v;
            next[v] = q;
            previous[v] = p;
            previous[q] = v;
            sign[p] = -1;
        }
    }
    Buffer<u32> rank(n);
    count = 0;
    for (u32 v = s; v != ~0u; v = next[v]) rank[v] = count++;
    if (count != n || rank[t] != n - 1) return {};
    for (u32 v = 0; v < n; ++v) {
        bool before = v == s, after = v == t;
        for (u32 w : g[v]) {
            before |= rank[w] < rank[v];
            after |= rank[w] > rank[v];
        }
        if (!before || !after) return {};
    }
    return rank;
}
} // namespace toy
