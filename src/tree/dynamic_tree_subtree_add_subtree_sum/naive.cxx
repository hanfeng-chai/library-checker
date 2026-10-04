#include <toy/io.h>
#include <toy/link_cut_subtree.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    auto [n, q] = in.read_pair<6>();
    Buffer<i64> a(n);
    for (auto &x : std::span(a.p, a.n)) x = in.read<u32, 8>();
    LinkCutSubtree<i64> tree{std::span<const i64>(a)};
    {
        Buffer<std::array<u32, 2>> edges(n - 1);
        for (auto &e : std::span(edges.p, edges.n)) {
            auto [u, v] = in.read_pair<6>();
            e = {u, v};
        }
        Adjacency graph(n, std::span<const std::array<u32, 2>>(edges));
        tree.build_tree(graph);
    }
    while (q--) {
        u32 type = in.read<u32, 1>();
        auto [u, v] = in.read_pair<6>();
        if (!type) {
            auto [w, x] = in.read_pair<6>();
            tree.cut(u, v);
            tree.link(w, x);
        } else if (type == 1)
            tree.side_add(u, v, in.read<u32, 8>());
        else
            out.write(tree.side_sum(u, v));
    }
}
