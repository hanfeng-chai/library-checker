#include <toy/io.h>
#include <toy/affine_point.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32, 6>(), q = in.read<u32, 6>(); Buffer<u32> a(n);
    for (auto& x : span(a.p, a.n)) x = in.read<u32, 16>();
    AffinePointTree tree{span<const u32>(a), q};
    while (q--) {
        u32 type = in.read<u32, 1>();
        if (!type) { auto [l, r] = in.read_pair<6>(); u32 a = in.read<u32, 16>(), b = in.read<u32, 16>(); tree.apply(l, r, {a, b}); }
        else tree.collect(in.read<u32, 6>());
    }
    out.write(tree.resolve());
}
