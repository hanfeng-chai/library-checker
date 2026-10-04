#include <toy/fenwick.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>(), q = in.read<u32, 6>();
    Buffer<u64> a(n, n + 1);
    for (auto &x : std::span(a.p, a.n)) x = in.read<u64, 10>();
    WideFenwick tree(std::move(a));
    while (q--) {
        auto type = in.read<u32, 1>();
        auto x = in.read<u32, 6>();
        if (!type)
            tree.add(x, in.read<u64, 10>());
        else
            out.write(tree.sum(x, in.read<u32, 6>()));
    }
}
