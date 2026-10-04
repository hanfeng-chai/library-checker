#pragma once
#include <toy/affine.h>
#include <toy/heavy_light.h>
#include <toy/montgomery.h>
namespace toy {
struct AffineEdge {
    u32 u, v, a, b;
};
template <u32 P = 998244353, bool Reroot = false>
struct TreeAffineUpdates {
    using R = Montgomery<P>;
    struct Back {
        u32 b = 0, c = 0;
    };
    struct Empty {};
    struct Segment {
        u32 a = R::one, b = 0, c = 0, count = 0;
        [[no_unique_address]] std::conditional_t<Reroot, Back, Empty> back;
    };
    struct Node {
        Segment sum;
        u32 a = R::one, b = 0, value = 0, virtual_sum = 0, count = 1, left = 0, right = 0,
            parent = 0;
    };
    u32 root = 0;
    Buffer<Node> nodes;
    Buffer<u32> position, edge_vertex;
    static u32 mixed(u32 a, u32 x, u32 b, u32 count) {
        u64 z = u64(a) * x + u64(b) * count;
        u32 y = (z + u64(u32(z) * Mod<P>::inverse) * P) >> 32;
        return std::min(y, y - 2 * P);
    }
    static Segment combine(Segment a, Segment b) {
        Segment c;
        c.a = R::multiply(a.a, b.a);
        c.b = R::add(R::multiply(a.a, b.b), a.b);
        c.c = R::add(mixed(a.a, b.c, a.b, b.count), a.c);
        c.count = a.count + b.count;
        if constexpr (Reroot)
            c.back = {R::add(R::multiply(b.a, a.back.b), b.back.b),
                      R::add(mixed(b.a, a.back.c, b.back.b, a.count), b.back.c)};
        return c;
    }
    Segment value(u32 i, u32 extra, u32 count) const {
        const auto &x = nodes[i];
        u32 sum = R::add(x.value, extra);
        Segment s;
        s.a = x.a;
        s.b = x.b;
        s.c = mixed(x.a, sum, x.b, count);
        s.count = count;
        if constexpr (Reroot) s.back = {x.b, sum};
        return s;
    }
    Segment value(u32 i) const { return value(i, nodes[i].virtual_sum, nodes[i].count); }
    void pull(u32 i) {
        auto &x = nodes[i];
        auto s = value(i);
        if (x.left) s = combine(nodes[x.left].sum, s);
        if (x.right) s = combine(s, nodes[x.right].sum);
        x.sum = s;
    }
    TreeAffineUpdates(std::span<const u32> values, std::span<const AffineEdge> edges, u32 base = 0)
        : nodes(values.size() + 1), edge_vertex(edges.size()) {
        u32 n = values.size();
        std::fill(nodes.p, nodes.p + nodes.n, Node{});
        nodes[0].count = 0;
        if (!n) return;
        HeavyLight h(n);
        for (auto e : edges) h.add_edge(e.u, e.v);
        h.build(base);
        for (u32 v = 0; v < n; ++v) {
            auto &x = nodes[h.position[v] + 1];
            x.value = values[v];
            x.count = h.size[v] - (h.heavy[v] == ~0u ? 0 : h.size[h.heavy[v]]);
        }
        for (u32 i = 0; i < edges.size(); ++i) {
            auto e = edges[i];
            u32 v = h.parent[e.u] == e.v ? e.u : e.v, id = edge_vertex[i] = h.position[v] + 1;
            nodes[id].a = R::encode(e.a);
            nodes[id].b = R::encode(e.b);
        }
        Buffer<u32> stack(n), degree(n + 1);
        Buffer<u8> priority(n + 1);
        std::fill(degree.p, degree.p + degree.n, 0u);
        for (u32 head = 0; head < n; ++head)
            if (h.head[head] == head) {
                u32 count = 0, start = 1;
                for (u32 v = head; v != ~0u; v = h.heavy[v]) {
                    u32 i = h.position[v] + 1, last = 0;
                    priority[i] = std::bit_width(start ^ (start + nodes[i].count)) - 1;
                    start += nodes[i].count;
                    while (count && priority[stack[count - 1]] < priority[i]) last = stack[--count];
                    if (count) {
                        nodes[stack[count - 1]].right = i;
                        nodes[i].parent = stack[count - 1];
                    }
                    nodes[i].left = last;
                    if (last) nodes[last].parent = i;
                    stack[count++] = i;
                }
                u32 r = stack[0];
                nodes[r].parent = (1u << 31) | (head == base ? 0 : h.position[h.parent[head]] + 1);
                if (head == base) root = r;
            }
        for (u32 v = 1; v <= n; ++v) ++degree[nodes[v].parent & 0x7fffffff];
        for (u32 start = 1; start <= n; ++start) {
            u32 v = start;
            while (v && !degree[v]) {
                pull(v);
                u32 p = nodes[v].parent & 0x7fffffff;
                if (nodes[v].parent >> 31)
                    nodes[p].virtual_sum = R::add(nodes[p].virtual_sum, nodes[v].sum.c);
                degree[v] = ~0u;
                --degree[p];
                v = p;
            }
        }
        position = std::move(h.position);
    }
    void update(u32 v) {
        while (v) {
            u32 old = nodes[v].sum.c, p = nodes[v].parent & 0x7fffffff;
            pull(v);
            if (nodes[v].parent >> 31)
                nodes[p].virtual_sum =
                    R::add(nodes[p].virtual_sum, R::subtract(nodes[v].sum.c, old));
            v = p;
        }
    }
    void set_vertex(u32 v, u32 x) {
        v = position[v] + 1;
        nodes[v].value = x;
        update(v);
    }
    void set_edge(u32 e, u32 a, u32 b) {
        u32 v = edge_vertex[e];
        nodes[v].a = R::encode(a);
        nodes[v].b = R::encode(b);
        update(v);
    }
    u32 root_sum() const {
        u32 x = nodes[root].sum.c;
        return std::min(x, x - P);
    }
    u32 sum(u32 vertex) const
        requires(Reroot)
    {
        u32 v = position[vertex] + 1, excluded = 0;
        Segment path;
        for (;;) {
            Segment above = nodes[nodes[v].left].sum, below = nodes[nodes[v].right].sum;
            u32 x = v;
            while (!(nodes[x].parent >> 31)) {
                u32 p = nodes[x].parent;
                const auto &y = nodes[p];
                if (x == y.left) {
                    auto s = value(p);
                    if (y.right) s = combine(s, nodes[y.right].sum);
                    below = combine(below, s);
                } else {
                    auto s = value(p);
                    if (y.left) s = combine(nodes[y.left].sum, s);
                    above = combine(s, above);
                }
                x = p;
            }
            auto own =
                value(v, R::add(R::subtract(nodes[v].virtual_sum, nodes[excluded].sum.c), below.c),
                      nodes[v].count - nodes[excluded].sum.count + below.count);
            path = combine(above, combine(own, path));
            u32 p = nodes[x].parent & 0x7fffffff;
            if (!p) {
                u32 answer = path.back.c;
                return std::min(answer, answer - P);
            }
            excluded = x;
            v = p;
        }
    }
};
} // namespace toy
