#pragma once
#include <toy/buffer.h>
namespace toy {
// Integral lower/upper bounds, signed costs and vertex supplies (out - in).
// Feasibility uses Dinic; epsilon scaling then cancels all negative residual
// cycles. Scaling costs by n+1 makes epsilon=1 sufficient for exact optimality.
struct MinCostFlow {
    struct Arc {
        u32 to, next;
        i64 capacity, cost;
    };
    u32 n, m, used = 0;
    Buffer<Arc> arc;
    Buffer<u32> head;
    Buffer<i64> balance, lower, potential, flow;
    i128 cost = 0;
    MinCostFlow(u32 n, u32 m)
        : n(n), m(m), arc(2 * (m + n)), head(n + 2), balance(n + 2), lower(m), potential(n + 2),
          flow(m) {
        std::fill(head.p, head.p + n + 2, ~0u);
        std::fill(balance.p, balance.p + n + 2, 0ll);
    }
    u32 link(u32 u, u32 v, i64 capacity, i64 cost) {
        u32 id = used;
        arc[used] = {v, head[u], capacity, cost};
        head[u] = used++;
        arc[used] = {u, head[v], 0, -cost};
        head[v] = used++;
        return id;
    }
    void add(u32 u, u32 v, i64 lo, i64 hi, i64 c) {
        lower[used / 2] = lo;
        balance[u] -= lo;
        balance[v] += lo;
        link(u, v, hi - lo, c);
    }
    bool solve() {
        i64 total = 0;
        for (u32 v = 0; v < n; ++v) total += balance[v];
        if (total) return false;
        u32 real = used;
        Buffer<u32> saved(n);
        memcpy(saved.p, head.p, n * 4);
        i64 needed = 0;
        for (u32 v = 0; v < n; ++v)
            if (balance[v] > 0) {
                link(n, v, balance[v], 0);
                needed += balance[v];
            } else if (balance[v] < 0)
                link(v, n + 1, -balance[v], 0);
        Buffer<i32> level(n + 2);
        Buffer<u32> queue(n + 2), current(n + 2);
        i64 sent = 0;
        while (sent < needed) {
            std::fill(level.p, level.p + n + 2, -1);
            u32 begin = 0, end = 1;
            queue[0] = n;
            level[n] = 0;
            while (begin < end) {
                u32 u = queue[begin++];
                for (u32 e = head[u]; e != ~0u; e = arc[e].next)
                    if (arc[e].capacity && level[arc[e].to] < 0) {
                        level[arc[e].to] = level[u] + 1;
                        queue[end++] = arc[e].to;
                    }
            }
            if (level[n + 1] < 0) return false;
            memcpy(current.p, head.p, (n + 2) * 4);
            auto dfs = [&](auto &&self, u32 u, i64 amount) -> i64 {
                if (u == n + 1) return amount;
                for (u32 &e = current[u]; e != ~0u; e = arc[e].next) {
                    auto &a = arc[e];
                    if (a.capacity && level[a.to] == level[u] + 1) {
                        i64 got = self(self, a.to, std::min(amount, a.capacity));
                        if (got) {
                            a.capacity -= got;
                            arc[e ^ 1].capacity += got;
                            return got;
                        }
                    }
                }
                return 0;
            };
            while (i64 pushed = dfs(dfs, n, needed - sent)) sent += pushed;
        }
        memcpy(head.p, saved.p, n * 4);
        used = real;
        std::fill(balance.p, balance.p + n, 0ll);
        std::fill(potential.p, potential.p + n, 0ll);
        i64 epsilon = 1;
        for (u32 e = 0; e < real; ++e) {
            arc[e].cost *= n + 1;
            while (epsilon < std::abs(arc[e].cost)) epsilon *= 8;
        }
        Buffer<u8> active(n);
        auto push = [&](u32 u, u32 e, i64 amount) {
            arc[e].capacity -= amount;
            arc[e ^ 1].capacity += amount;
            balance[u] -= amount;
            balance[arc[e].to] += amount;
        };
        while (epsilon > 1) {
            epsilon = std::max<i64>(1, epsilon / 8);
            for (u32 u = 0; u < n; ++u)
                for (u32 e = head[u]; e != ~0u; e = arc[e].next)
                    if (arc[e].capacity && arc[e].cost + potential[u] - potential[arc[e].to] < 0)
                        push(u, e, arc[e].capacity);
            memcpy(current.p, head.p, n * 4);
            std::fill(active.p, active.p + n, 0);
            u32 begin = 0, end = 0;
            auto enqueue = [&](u32 v) {
                if (balance[v] > 0 && !active[v]) {
                    active[v] = 1;
                    queue[end] = v;
                    if (++end == queue.n) end = 0;
                }
            };
            for (u32 v = 0; v < n; ++v) enqueue(v);
            while (begin != end) {
                u32 u = queue[begin];
                if (++begin == queue.n) begin = 0;
                active[u] = 0;
                while (balance[u] > 0) {
                    u32 &e = current[u];
                    while (e != ~0u) {
                        auto &a = arc[e];
                        if (a.capacity && a.cost + potential[u] - potential[a.to] < 0) {
                            push(u, e, std::min(balance[u], a.capacity));
                            enqueue(a.to);
                            if (!balance[u]) break;
                        }
                        e = a.next;
                    }
                    if (balance[u]) {
                        i64 best = INT64_MAX;
                        for (u32 e = head[u]; e != ~0u; e = arc[e].next)
                            if (arc[e].capacity)
                                best = std::min(best,
                                                arc[e].cost + potential[u] - potential[arc[e].to]);
                        potential[u] -= best + epsilon;
                        current[u] = head[u];
                    }
                }
            }
        }
        for (u32 e = 0; e < real; ++e) arc[e].cost /= n + 1;
        // Exact feasible dual for the original (unscaled) residual costs.
        std::fill(potential.p, potential.p + n, 0ll);
        for (u32 pass = 0; pass < n; ++pass) {
            bool changed = false;
            for (u32 u = 0; u < n; ++u)
                for (u32 e = head[u]; e != ~0u; e = arc[e].next)
                    if (arc[e].capacity && potential[arc[e].to] > potential[u] + arc[e].cost) {
                        potential[arc[e].to] = potential[u] + arc[e].cost;
                        changed = true;
                    }
            if (!changed) break;
        }
        for (u32 i = 0; i < m; ++i) {
            flow[i] = lower[i] + arc[2 * i + 1].capacity;
            cost += i128(flow[i]) * arc[2 * i].cost;
        }
        return true;
    }
};
} // namespace toy
