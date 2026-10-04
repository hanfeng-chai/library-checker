#pragma once
#include <toy/adjacency.h>
namespace toy {
struct CentroidPoint {
    u32 vertex, distance;
};
// Callback gets the center at points[0], then one contiguous interval per remaining branch.
// Scratch spans are valid only until the callback returns.
template <class Visit>
Buffer<u32> centroid_decompose(const Adjacency &graph, Visit visit) {
    u32 n = graph.size();
    Buffer<u32> result(n), parent(n), size(n), order(n), starts(n + 1);
    Buffer<u8> removed(n);
    std::fill(removed.p, removed.p + n, 0);
    struct Task {
        u32 root, parent;
    };
    Buffer<Task> tasks(n);
    tasks.n = 0;
    if (n) tasks[tasks.n++] = {0, ~0u};
    Buffer<CentroidPoint> points(n);
    while (tasks.n) {
        auto task = tasks[--tasks.n];
        u32 count = 1;
        order[0] = task.root;
        parent[task.root] = ~0u;
        for (u32 i = 0; i < count; ++i) {
            u32 v = order[i];
            size[v] = 1;
            for (u32 w : graph[v])
                if (!removed[w] && w != parent[v]) {
                    parent[w] = v;
                    order[count++] = w;
                }
        }
        for (u32 i = count; i-- > 1;) size[parent[order[i]]] += size[order[i]];
        u32 center = task.root;
        for (;;) {
            u32 next = ~0u;
            for (u32 w : graph[center])
                if (!removed[w] && parent[w] == center && size[w] > count / 2) {
                    next = w;
                    break;
                }
            if (next == ~0u) break;
            center = next;
        }
        result[center] = task.parent;
        removed[center] = 1;
        points.n = 1;
        points[0] = {center, 0};
        starts.n = 1;
        starts[0] = 1;
        for (u32 child : graph[center])
            if (!removed[child]) {
                u32 first = points.n;
                points[points.n++] = {child, 1};
                parent[child] = center;
                tasks[tasks.n++] = {child, center};
                for (u32 i = first; i < points.n; ++i) {
                    auto x = points[i];
                    for (u32 w : graph[x.vertex])
                        if (!removed[w] && w != parent[x.vertex]) {
                            parent[w] = x.vertex;
                            points[points.n++] = {w, x.distance + 1};
                        }
                }
                starts[starts.n++] = points.n;
            }
        visit(center, std::span<const CentroidPoint>(points), std::span<const u32>(starts));
    }
    return result;
}
} // namespace toy
