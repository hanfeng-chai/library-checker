#include <toy/fenwick.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    auto [n, q] = in.read_pair<6>();
    Buffer<u64> a(n);
    for (auto &x : std::span(a.p, a.n)) x = in.read<u64, 10>();
    Fenwick<u64> tree{std::span<const u64>(a)};
    while (q--) {
        u32 type = in.read<u32, 1>(), x = in.read<u32, 6>();
        if (!type)
            tree.add(x, in.read<u64, 10>());
        else
            out.write(tree.sum(x, in.read<u32, 6>()));
    }
}
