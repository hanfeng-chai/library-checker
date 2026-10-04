#include <toy/io.h>
#include <toy/tree_affine_updates.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    auto [n, q] = in.read_pair<6>();
    Buffer<u32> a(n);
    for (u32 i = 0; i + 1 < n; i += 2) {
        auto [x, y] = in.read_pair<9>();
        a[i] = x;
        a[i + 1] = y;
    }
    if (n & 1) a[n - 1] = in.read<u32, 9>();
    Buffer<AffineEdge> edges(n - 1);
    for (auto &e : std::span(edges.p, edges.n)) {
        auto [u, v] = in.read_pair<6>();
        auto [b, c] = in.read_pair<9>();
        e = {u, v, b, c};
    }
    TreeAffineUpdates<998244353, true> tree{std::span<const u32>(a),
                                            std::span<const AffineEdge>(edges)};
    while (q--) {
        auto [type, id] = in.read_pair<6>();
        if (!type)
            tree.set_vertex(id, in.read<u32, 9>());
        else {
            auto [b, c] = in.read_pair<9>();
            tree.set_edge(id, b, c);
        }
        out.write(tree.sum(in.read<u32, 6>()));
    }
}
