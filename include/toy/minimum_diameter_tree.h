#pragma once
#include <toy/graph.h>
#include <toy/radix_heap.h>
namespace toy {
struct MinimumDiameterTree {
    u64 diameter;
    Buffer<u32> edge;
};
template <bool Witness = true>
MinimumDiameterTree minimum_diameter_tree(u32 n, std::span<const std::array<u32, 3>> edges) {
    if (n == 1) return {0, {}};
    auto g = Graph<IndexedArc>(n, edges, false, [](u32 to, u32 id) { return IndexedArc{to, id}; });
    struct Center {
        u64 diameter = ~0ull, offset = 0;
        u32 u = 0, v = 0, edge = ~0u;
    };
    Center best;
    Buffer<u64> distance(n);
    Buffer<u32> parent(n), order(0, n);
    RadixHeap queue;
    auto run = [&](u32 u, u64 a, u32 v, u64 b, u32 scale) {
        std::fill(distance.p, distance.p + n, ~0ull);
        std::fill(parent.p, parent.p + n, ~0u);
        queue.clear();
        order.n = 0;
        distance[u] = a;
        queue.push(a, u);
        if (b < distance[v]) {
            distance[v] = b;
            queue.push(b, v);
        }
        while (!queue.empty()) {
            auto [d, x] = queue.pop();
            if (d != distance[x]) continue;
            order.p[order.n++] = x;
            for (auto e : g[x]) {
                u64 next = d + u64(edges[e.id][2]) * scale;
                if (next < distance[e.to]) {
                    distance[e.to] = next;
                    parent[e.to] = e.id;
                    queue.push(next, e.to);
                }
            }
        }
    };
    if (edges.size() + 1 == n) {
        run(0, 0, 0, 0, 1);
        u32 far = order[order.n - 1];
        run(far, 0, far, 0, 1);
        Buffer<u32> ids(edges.size());
        std::iota(ids.p, ids.p + ids.n, 0u);
        return {distance[order[order.n - 1]], std::move(ids)};
    }
    Buffer<u32> active(0, edges.size());
    for (u32 i = 0; i < edges.size(); ++i)
        if (edges[i][0] != edges[i][1]) active.p[active.n++] = i;
    bool certified = false;
    if constexpr (Witness) {
        struct Point {
            u64 a, b;
        };
        struct Envelope {
            Point point[16];
            u32 size = 0;
        };
        Buffer<Envelope> envelope(edges.size());
        for (auto &e : std::span(envelope.p, envelope.n)) e.size = 0;
        u32 witness = 0;
        for (u32 round = 0; round < 16; ++round) {
            run(witness, 0, witness, 0, 1);
            Center candidate;
            u32 kept = 0;
            for (u32 id : std::span(active.p, active.n)) {
                auto edge = edges[id];
                auto &e = envelope[id];
                Point x{distance[edge[0]], distance[edge[1]]};
                u32 at = 0;
                while (at < e.size && e.point[at].a < x.a) ++at;
                if (at == e.size || e.point[at].b < x.b) {
                    u32 end = at;
                    if (end < e.size && e.point[end].a == x.a) ++end;
                    while (at && e.point[at - 1].b <= x.b) --at;
                    memmove(e.point + at + 1, e.point + end, (e.size - end) * sizeof(Point));
                    e.size -= end - at;
                    e.point[at] = x;
                    ++e.size;
                }
                u64 diameter = 2 * e.point[e.size - 1].a, offset = 0, w = edge[2];
                if (2 * e.point[0].b < diameter) {
                    diameter = 2 * e.point[0].b;
                    offset = 2 * w;
                }
                for (u32 i = 1; i < e.size; ++i) {
                    u64 a = e.point[i - 1].a, b = e.point[i].b;
                    if (a <= b + w && b <= a + w && a + b + w < diameter) {
                        diameter = a + b + w;
                        offset = b + w - a;
                    }
                }
                if (diameter >= best.diameter) continue;
                active[kept++] = id;
                if (diameter < candidate.diameter)
                    candidate = {diameter, offset, edge[0], edge[1], id};
            }
            active.n = kept;
            if (!kept) {
                certified = true;
                break;
            }
            run(candidate.u, candidate.offset, candidate.v,
                2ull * edges[candidate.edge][2] - candidate.offset, 2);
            witness = order[order.n - 1];
            u64 actual = distance[witness], bound = candidate.diameter;
            candidate.diameter = actual;
            if (actual < best.diameter) best = candidate;
            if (actual == bound) {
                certified = true;
                break;
            }
        }
    }
    if (!certified) {
        // Exact fallback: all-pairs distances, scanning each edge's envelope in
        // increasing distance from its first endpoint. No heuristic cutoff.
        Buffer<u64> matrix(usize(n) * n);
        Buffer<u8> live(edges.size());
        memset(live.p, 0, live.n);
        for (u32 id : std::span(active.p, active.n)) live[id] = 1;
        u64 lower = 0;
        for (u32 u = 0; u < n; ++u) {
            run(u, 0, u, 0, 1);
            u64 *du = matrix.p + usize(u) * n;
            memcpy(du, distance.p, n * 8);
            u64 eccentricity = du[order[order.n - 1]];
            lower = std::max(lower, eccentricity);
            if (2 * eccentricity < best.diameter) best = {2 * eccentricity, 0, u, u, ~0u};
            for (auto arc : g[u])
                if (live[arc.id] && arc.to < u) {
                    u32 v = arc.to;
                    u64 w = edges[arc.id][2], *dv = matrix.p + usize(v) * n,
                        maximum = dv[order[order.n - 1]];
                    for (u32 i = n - 1; i--;) {
                        u32 x = order[i];
                        if (dv[x] <= maximum) continue;
                        if (du[x] <= maximum + w && maximum <= du[x] + w &&
                            du[x] + maximum + w < best.diameter)
                            best = {du[x] + maximum + w, maximum + w - du[x], u, v, arc.id};
                        maximum = dv[x];
                    }
                }
            if (best.diameter == lower) break;
        }
    }
    u64 other = best.edge == ~0u ? 0 : 2ull * edges[best.edge][2] - best.offset;
    run(best.u, best.offset, best.v, other, 2);
    MinimumDiameterTree result{best.diameter, Buffer<u32>(0, n - 1)};
    for (u32 id : std::span(parent.p, parent.n))
        if (id != ~0u) result.edge.p[result.edge.n++] = id;
    if (result.edge.n + 2 == n) result.edge.p[result.edge.n++] = best.edge;
    return result;
}
} // namespace toy
