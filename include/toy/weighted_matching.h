#pragma once
#include <toy/array.h>
#include <toy/buffer.h>
#include <toy/page_buffer.h>
namespace toy {
// Primal-dual Edmonds blossom algorithm for maximum-weight matching.
// Positive u32 weights, n < 32768. Vertex IDs inside blossoms fit in u16.
struct WeightedMatching {
    struct Edge {
        u16 u, v;
        u32 weight;
    };
    u32 n, limit, count, stamp = 0;
    Buffer<Edge> edge;
    Buffer<u16> inside;
    Array<Buffer<u32>> flower;
    Buffer<u32> mate, slack, base, parent, seen, queue, active, roots, slack_roots;
    Buffer<u8> active_seen, root_seen, slack_seen;
    Buffer<i8> state;
    Buffer<i64> label, slack_value;
    WeightedMatching(u32 n)
        : n(n), limit(2 * n + 1), count(n), edge(page_buffer<Edge>(usize(limit) * limit)),
          inside(page_buffer<u16>(usize(limit) * limit)), flower(limit), mate(limit), slack(limit),
          base(limit), parent(limit), seen(limit), active(0, n), roots(0, limit),
          slack_roots(0, limit), active_seen(n + 1), root_seen(limit), slack_seen(limit),
          state(limit), label(limit), slack_value(limit) {
        std::fill(edge.p, edge.p + edge.n, Edge{});
        std::fill(inside.p, inside.p + inside.n, 0);
        std::fill(mate.p, mate.p + limit, 0u);
        std::fill(base.p, base.p + limit, 0u);
        std::fill(seen.p, seen.p + limit, 0u);
        std::fill(label.p, label.p + limit, 0ll);
        std::fill(parent.p, parent.p + limit, 0u);
        for (u32 u = 1; u <= n; ++u) {
            base[u] = u;
            part(u, u) = u;
            for (u32 v = 1; v <= n; ++v) at(u, v) = {u16(u), u16(v), 0};
        }
    }
    Edge &at(u32 u, u32 v) { return edge[usize(u) * limit + v]; }
    u16 &part(u32 u, u32 v) { return inside[usize(u) * limit + v]; }
    void add(u32 u, u32 v, u32 w) {
        ++u;
        ++v;
        if (u == v) return;
        at(u, v).weight = at(v, u).weight = std::max(at(u, v).weight, w);
    }
    i64 delta(Edge e) const { return label[e.u] + label[e.v] - 2ll * e.weight; }
    void touch_root(u32 v) {
        if (!root_seen[v]) {
            root_seen[v] = 1;
            roots.p[roots.n++] = v;
        }
    }
    void touch(u32 v) {
        if (v <= n) {
            if (!active_seen[v]) {
                active_seen[v] = 1;
                active.p[active.n++] = v;
            }
        } else
            for (u32 u : std::span(flower[v].p, flower[v].n)) touch(u);
    }
    void update_slack(u32 u, u32 v, i64 value) {
        if (!slack_seen[v]) {
            slack_seen[v] = 1;
            slack_roots.p[slack_roots.n++] = v;
        }
        if (!slack[v] || value < slack_value[v]) {
            slack[v] = u;
            slack_value[v] = value;
        }
    }
    void update_slack(u32 u, u32 v) { update_slack(u, v, delta(at(u, v))); }
    void reset_slack(u32 v) {
        slack[v] = 0;
        for (u32 u = 1; u <= n; ++u)
            if (at(u, v).weight && base[u] != v && !state[base[u]]) update_slack(u, v);
    }
    static void append(Buffer<u32> &a, u32 x) {
        if (a.n == a.capacity) a.reserve(std::max<usize>(8, a.capacity * 2));
        a.p[a.n++] = x;
    }
    void enqueue(u32 u) {
        if (u <= n) {
            touch(u);
            append(queue, u);
        } else
            for (u32 v : std::span(flower[u].p, flower[u].n)) enqueue(v);
    }
    void set_base(u32 u, u32 b) {
        base[u] = b;
        if (u > n)
            for (u32 v : std::span(flower[u].p, flower[u].n)) set_base(v, b);
    }
    u32 position(u32 u, u32 v) {
        auto &f = flower[u];
        u32 p = std::find(f.p, f.p + f.n, v) - f.p;
        if (p & 1) {
            std::reverse(f.p + 1, f.p + f.n);
            p = f.n - p;
        }
        return p;
    }
    void set_match(u32 u, u32 v) {
        Edge e = at(u, v);
        mate[u] = e.v;
        if (u <= n) return;
        u32 x = part(u, e.u), p = position(u, x);
        auto &f = flower[u];
        for (u32 i = 0; i < p; ++i) set_match(f[i], f[i ^ 1]);
        set_match(x, v);
        std::rotate(f.p, f.p + p, f.p + f.n);
    }
    void augment(u32 u, u32 v) {
        u32 next = base[mate[u]];
        set_match(u, v);
        if (next) {
            set_match(next, base[parent[next]]);
            augment(base[parent[next]], next);
        }
    }
    u32 lca(u32 u, u32 v) {
        ++stamp;
        while (u || v) {
            if (u) {
                if (seen[u] == stamp) return u;
                seen[u] = stamp;
                u = base[mate[u]];
                if (u) u = base[parent[u]];
            }
            std::swap(u, v);
        }
        return 0;
    }
    void contract(u32 u, u32 common, u32 v) {
        u32 b = n + 1;
        while (b <= count && base[b]) ++b;
        if (b > count) ++count;
        label[b] = 0;
        state[b] = 0;
        mate[b] = mate[common];
        auto &f = flower[b];
        f.n = 0;
        append(f, common);
        auto path = [&](u32 x) {
            while (x != common) {
                append(f, x);
                u32 y = base[mate[x]];
                append(f, y);
                enqueue(y);
                x = base[parent[y]];
            }
        };
        path(u);
        std::reverse(f.p + 1, f.p + f.n);
        path(v);
        set_base(b, b);
        touch_root(b);
        for (u32 x = 1; x <= count; ++x) at(b, x).weight = at(x, b).weight = 0;
        std::fill(inside.p + usize(b) * limit, inside.p + usize(b) * limit + n + 1, 0);
        for (u32 x : std::span(f.p, f.n)) {
            for (u32 y = 1; y <= count; ++y)
                if (!at(b, y).weight || delta(at(x, y)) < delta(at(b, y))) {
                    at(b, y) = at(x, y);
                    at(y, b) = at(y, x);
                }
            for (u32 y = 1; y <= n; ++y)
                if (part(x, y)) part(b, y) = x;
        }
        reset_slack(b);
    }
    void expand(u32 b) {
        auto &f = flower[b];
        for (u32 x : std::span(f.p, f.n)) set_base(x, x);
        u32 x = part(b, at(b, parent[b]).u), p = position(b, x);
        for (u32 i = 0; i < p; i += 2) {
            u32 u = f[i], v = f[i + 1];
            parent[u] = at(v, u).u;
            state[u] = 1;
            state[v] = 0;
            touch_root(u);
            touch_root(v);
            slack[u] = 0;
            reset_slack(v);
            enqueue(v);
        }
        state[x] = 1;
        parent[x] = parent[b];
        touch_root(x);
        for (u32 i = p + 1; i < f.n; ++i) {
            state[f[i]] = -1;
            reset_slack(f[i]);
        }
        base[b] = 0;
    }
    bool tight(Edge e) {
        u32 u = base[e.u], v = base[e.v];
        if (state[v] < 0) {
            parent[v] = e.u;
            state[v] = 1;
            touch_root(v);
            touch(v);
            u32 next = base[mate[v]];
            slack[v] = slack[next] = 0;
            state[next] = 0;
            touch_root(next);
            enqueue(next);
        } else if (!state[v]) {
            u32 common = lca(u, v);
            if (!common) {
                augment(u, v);
                augment(v, u);
                return true;
            }
            contract(u, common, v);
        }
        return false;
    }
    bool increase() {
        std::fill(state.p, state.p + count + 1, -1);
        std::fill(slack.p, slack.p + count + 1, 0u);
        queue.n = active.n = roots.n = slack_roots.n = 0;
        std::fill(active_seen.p, active_seen.p + n + 1, 0);
        std::fill(root_seen.p, root_seen.p + limit, 0);
        std::fill(slack_seen.p, slack_seen.p + limit, 0);
        for (u32 u = 1; u <= count; ++u)
            if (base[u] == u && !mate[u]) {
                parent[u] = 0;
                state[u] = 0;
                touch_root(u);
                enqueue(u);
            }
        if (!queue.n) return false;
        u32 head = 0;
        for (;;) {
            while (head < queue.n) {
                u32 u = queue[head++];
                if (state[base[u]] == 1) continue;
                const Edge *row = edge.p + usize(u) * limit;
                i64 value = label[u];
                // Edges between original vertices never change during contraction.
                // Use their known endpoints directly instead of dependent loads.
                for (u32 v = 1; v <= n; ++v) {
                    u32 w = row[v].weight;
                    if (w && base[u] != base[v]) {
                        i64 reduced = value + label[v] - 2ll * w;
                        if (reduced) {
                            u32 b = base[v];
                            update_slack(u, b, b == v ? reduced : delta(at(u, b)));
                        } else if (tight({u16(u), u16(v), w}))
                            return true;
                    }
                }
            }
            i64 change = INT64_MAX;
            for (u32 b : std::span(roots.p, roots.n))
                if (b > n && base[b] == b && state[b] == 1) change = std::min(change, label[b] / 2);
            for (u32 b : std::span(slack_roots.p, slack_roots.n))
                if (base[b] == b && slack[b] && state[b] != 1)
                    change = std::min(change, slack_value[b] / (state[b] == 0 ? 2 : 1));
            for (u32 u : std::span(active.p, active.n))
                if (!state[base[u]] && label[u] <= change) return false;
            for (u32 u : std::span(active.p, active.n))
                if (state[base[u]] >= 0) label[u] += (2 * state[base[u]] - 1) * change;
            for (u32 b : std::span(roots.p, roots.n))
                if (b > n && base[b] == b && state[b] >= 0) label[b] += (2 - 4 * state[b]) * change;
            queue.n = head = 0;
            // Labels stay fixed throughout a BFS. Refresh each winning slack
            // once after a dual update instead of reloading its edge per probe.
            for (u32 b : std::span(slack_roots.p, slack_roots.n))
                if (base[b] == b && slack[b]) slack_value[b] = delta(at(slack[b], b));
            for (u32 b = 1; b <= count; ++b)
                if (base[b] == b && slack[b] && base[slack[b]] != b && !slack_value[b] &&
                    tight(at(slack[b], b)))
                    return true;
            for (u32 b = n + 1; b <= count; ++b)
                if (base[b] == b && state[b] == 1 && !label[b]) expand(b);
        }
    }
    u64 solve() {
        u32 maximum = 0;
        for (u32 u = 1; u <= n; ++u)
            for (u32 v = 1; v <= n; ++v) maximum = std::max(maximum, at(u, v).weight);
        std::fill(label.p + 1, label.p + n + 1, maximum);
        if (maximum)
            while (increase());
        u64 answer = 0;
        for (u32 u = 1; u <= n; ++u)
            if (mate[u] > u) answer += at(u, mate[u]).weight;
        return answer;
    }
};
} // namespace toy
