#include <toy/io.h>
#include <toy/multipoint.h>
using namespace toy;
int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<u32> f(n), points(m);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    while (*in.p && *in.p <= ' ') ++in.p;
    for (usize i = 0; i < m; ++i) points[i] = in.read<u32, 9>();
    Multipoint tree{std::span<const u32>(points)};
    auto values = tree.evaluate(std::span<const u32>(f));
    out.write(std::span(values.p, values.n), ' ');
}
