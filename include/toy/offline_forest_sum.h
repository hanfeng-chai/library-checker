#pragma once
#include <toy/hash.h>
namespace toy {
// Edges expire in decreasing weight order. Internal links encode threshold connectivity,
// not the original tree's edges; arbitrary online deletions would be invalid.
template <class T = i64, bool RangeAdd = false>
struct ExpiryForest {
    struct Tag {
        T add{};
    };
    struct Empty {};
    struct Node {
        T sum{};
        [[no_unique_address]] std::conditional_t<RangeAdd, Tag, Empty> tag;
        u32 parent = 0, size = 1;
        i32 weight = INT_MAX;
    };
    Buffer<Node> nodes;
    explicit ExpiryForest(std::span<const T> a) : nodes(a.size() + 1) {
        std::fill(nodes.p, nodes.p + nodes.n, Node{});
        nodes[0].size = 0;
        for (u32 i = 0; i < a.size(); ++i) nodes[i + 1].sum = a[i];
    }
    T offset(u32 v) const {
        if constexpr (RangeAdd)
            return nodes[v].tag.add;
        else
            return T{};
    }
    void apply(u32 v, T x) {
        if constexpr (RangeAdd) {
            nodes[v].tag.add += x;
            nodes[v].sum += x * nodes[v].size;
        }
    }
    void promote(u32 v) {
        u32 p = nodes[v].parent, len = nodes[v].size;
        T x = offset(p);
        nodes[p].size -= len;
        nodes[p].sum -= nodes[v].sum + x * len;
        nodes[v].parent = nodes[p].parent;
        apply(v, x);
        if (nodes[v].weight < nodes[p].weight) {
            x = offset(v);
            apply(p, -x);
            nodes[v].size += nodes[p].size;
            nodes[v].sum += nodes[p].sum + x * nodes[p].size;
            std::swap(nodes[v].weight, nodes[p].weight);
            nodes[p].parent = v;
        }
    }
    void maintain(u32 v) {
        while (u32 p = nodes[v].parent) {
            if (3ull * nodes[v].size <= 2ull * nodes[p].size)
                v = p;
            else
                promote(v);
        }
    }
    u32 root(u32 v) const {
        while (nodes[v].parent) v = nodes[v].parent;
        return v;
    }
    T potential(u32 v) const {
        T x{};
        if constexpr (RangeAdd)
            for (; v; v = nodes[v].parent) x += offset(v);
        return x;
    }
    void link(u32 u, u32 v, i32 w) {
        ++u;
        ++v;
        maintain(u);
        maintain(v);
        T pu = potential(u), pv = potential(v), su{}, sv{};
        i32 du = 0, dv = 0;
        for (;;) {
            if (w >= nodes[u].weight) {
                u32 p = nodes[u].parent;
                su += offset(p) * du;
                nodes[p].size += du;
                nodes[p].sum += su;
                pu -= offset(u);
                u = p;
            } else if (w >= nodes[v].weight) {
                u32 p = nodes[v].parent;
                sv += offset(p) * dv;
                nodes[p].size += dv;
                nodes[p].sum += sv;
                pv -= offset(v);
                v = p;
            } else {
                if (nodes[u].size > nodes[v].size) {
                    std::swap(u, v);
                    std::swap(du, dv);
                    std::swap(su, sv);
                    std::swap(pu, pv);
                }
                u32 len = nodes[u].size, p = nodes[u].parent;
                i32 old = nodes[u].weight;
                T value = nodes[u].sum, tag = offset(u);
                if (p) {
                    T next = su + offset(p) * du;
                    du -= len;
                    su = next - value - offset(p) * len;
                    nodes[p].size += du;
                    nodes[p].sum += su;
                }
                apply(u, pu - tag - pv);
                T contribution = nodes[u].sum + offset(v) * len;
                dv += len;
                sv += contribution;
                nodes[v].size += len;
                nodes[v].sum += contribution;
                nodes[u].parent = v;
                nodes[u].weight = w;
                w = old;
                if (!p) {
                    while (nodes[v].parent) {
                        u32 next = nodes[v].parent;
                        sv += offset(next) * dv;
                        nodes[next].size += dv;
                        nodes[next].sum += sv;
                        v = next;
                    }
                    return;
                }
                pu -= tag;
                u = p;
            }
        }
    }
    void cut(u32 u, u32 v) {
        ++u;
        ++v;
        maintain(u);
        maintain(v);
        u32 target = 0;
        while (u != v) {
            if (nodes[u].size > nodes[v].size) std::swap(u, v);
            if (!target || nodes[u].weight > nodes[target].weight) target = u;
            u = nodes[u].parent;
        }
        u32 len = nodes[target].size, p = nodes[target].parent;
        T accumulated = potential(p), delta = -nodes[target].sum - offset(p) * len;
        while (p) {
            nodes[p].size -= len;
            nodes[p].sum += delta;
            p = nodes[p].parent;
            delta -= offset(p) * len;
        }
        apply(target, accumulated);
        nodes[target].parent = 0;
        nodes[target].weight = INT_MAX;
    }
    void add_vertex(u32 v, T x) {
        maintain(++v);
        for (; v; v = nodes[v].parent) nodes[v].sum += x;
    }
    void add_component(u32 v, T x)
        requires(RangeAdd)
    {
        maintain(++v);
        apply(root(v), x);
    }
    T sum(u32 v) {
        maintain(++v);
        return nodes[root(v)].sum;
    }
};

template <class T = i64, bool RangeAdd = false>
struct OfflineForestSum {
    struct Event {
        T value;
        u32 u, v, type;
    };
    Buffer<Event> events;
    HashMap<u64, u32> active;
    u32 answers = 0;
    OfflineForestSum(u32 vertices, u32 operations)
        : events(0, vertices + 3 * operations), active(vertices + operations) {}
    static u64 key(u32 u, u32 v) { return (u64(std::min(u, v)) << 32) | std::max(u, v); }
    void link(u32 u, u32 v) {
        active[key(u, v)] = events.n;
        events[events.n++] = {INT_MIN, u, v, 0};
    }
    void cut(u32 u, u32 v) {
        events[active.get(key(u, v))].value = -i64(events.n);
        events[events.n++] = {0, u, v, 1};
    }
    void add_vertex(u32 v, T x) { events[events.n++] = {x, v, 0, 2}; }
    void add_component(u32 v, T x)
        requires(RangeAdd)
    {
        events[events.n++] = {x, v, 0, 3};
    }
    u32 sum(u32 v) {
        events[events.n++] = {0, v, 0, 4};
        return answers++;
    }
    Buffer<T> solve(std::span<const T> values) const {
        ExpiryForest<T, RangeAdd> tree(values);
        Buffer<T> result(answers);
        u32 at = 0;
        for (auto e : std::span(events.p, events.n)) {
            if (e.type == 0)
                tree.link(e.u, e.v, e.value);
            else if (e.type == 1)
                tree.cut(e.u, e.v);
            else if (e.type == 2)
                tree.add_vertex(e.u, e.value);
            else if constexpr (RangeAdd) {
                if (e.type == 3)
                    tree.add_component(e.u, e.value);
                else
                    result[at++] = tree.sum(e.u);
            } else
                result[at++] = tree.sum(e.u);
        }
        return result;
    }
};
} // namespace toy
