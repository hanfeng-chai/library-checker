#include <toy/fenwick.h>
#include <toy/heavy_light.h>
#include <toy/io.h>
#include <toy/ordered_lca.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    auto [n, q] = in.read_pair<6>();
    Buffer<u64> weights(n), initial(n);
    struct Op {
        u32 type, a, b, c;
    };
    Buffer<Op> ops(q);
    {
        Buffer<u64> a(n);
        for (auto &x : std::span(a.p, a.n)) x = in.read<u32, 10>();
        HeavyLight h(n);
        for (u32 i = 1; i < n; ++i) {
            auto [u, v] = in.read_pair<6>();
            h.add_edge(u, v);
        }
        h.build();
        Buffer<u32> parent(n);
        for (u32 i = 0; i < n; ++i) {
            u32 v = h.vertex[i];
            parent[i] = h.position[h.parent[v]];
            weights[i] = a[v];
            initial[i] = a[v] + (i ? initial[parent[i]] : 0);
        }
        OrderedLCA lca{std::span<const u32>(parent)};
        for (auto &op : std::span(ops.p, ops.n)) {
            op.type = in.read<u32, 1>();
            u32 u = in.read<u32, 6>();
            op.a = h.position[u];
            if (!op.type) {
                op.b = op.a + h.size[u];
                op.c = in.read<u32, 10>();
            } else {
                op.b = h.position[in.read<u32, 6>()];
                if (op.a > op.b) std::swap(op.a, op.b);
                op.c = lca.lca(op.a, op.b);
            }
        }
    }
    for (u32 i = n; --i;) initial[i] -= initial[i - 1];
    WideFenwick sum(std::move(initial));
    for (auto op : std::span(ops.p, ops.n)) {
        if (!op.type) {
            weights[op.a] += op.c;
            sum.add_difference(op.a, op.b, op.c);
        } else if (op.a == op.b)
            out.write(weights[op.a]);
        else if (op.a == op.c)
            out.write(sum.sum(op.a + 1, op.b + 1) + weights[op.c]);
        else
            out.write(sum.prefix(op.a + 1) + sum.prefix(op.b + 1) - 2 * sum.prefix(op.c + 1) +
                      weights[op.c]);
    }
}
