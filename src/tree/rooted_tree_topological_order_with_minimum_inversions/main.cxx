#include <toy/io.h>
#include <toy/tree_order.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>();
    Buffer<u32> p(n), c(n), d(n);
    p[0] = 0;
    for (u32 i = 1; i < n; ++i) p[i] = in.read<u32, 6>();
    if (n == 1)
        while (*in.p <= ' ') ++in.p;
    for (auto &x : std::span(c.p, c.n)) x = in.read<u32, 10>();
    for (auto &x : std::span(d.p, d.n)) x = in.read<u32, 10>();
    auto result = minimum_tree_order(std::span<const u32>(p), std::span<const u32>(c),
                                     std::span<const u32>(d));
    out.write(result.cost);
    out.write(std::span(result.order.p, result.order.n));
}
