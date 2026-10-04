#include <toy/io.h>
#include <toy/tree_affine_sum.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>();
    Buffer<u32> a(n);
    for (u32 i = 0; i + 1 < n; i += 2) {
        auto [x, y] = in.read_pair<9>();
        a[i] = x;
        a[i + 1] = y;
    }
    if (n & 1) a[n - 1] = in.read<u32, 9>();
    TreeAffineSum tree{std::span<const u32>(a)};
    for (u32 i = 1; i < n; ++i) {
        auto [u, v] = in.read_pair<6>();
        auto [b, c] = in.read_pair<9>();
        tree.add_edge(u, v, b, c);
    }
    auto result = tree.solve();
    out.write(std::span(result.p, result.n));
}
