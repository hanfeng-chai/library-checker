#include <toy/io.h>
#include <toy/linear_min.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), q = in.read<u32, 6>();
    Buffer<i64> a(n);
    for (auto &x : std::span(a.p, a.n)) x = in.read<i64, 16>();
    LinearAddMin tree{std::span<const i64>(a)};
    while (q--) {
        u32 type = in.read<u32, 1>();
        auto [l, r] = in.read_pair<6>();
        if (!type) {
            i64 b = in.read<i64, 4>(), c = in.read<i64, 8>();
            tree.add(l, r, b, c);
        } else
            out.write(tree.minimum(l, r));
    }
}
