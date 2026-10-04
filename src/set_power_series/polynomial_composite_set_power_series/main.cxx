#include <toy/io.h>
#include <toy/set_series.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    usize m = in.read<u32, 6>(), n = usize(1) << in.read<u32, 2>();
    Buffer<u32> f(m), g(n);
    for (usize i = 0; i < m; ++i) f[i] = in.read<u32, 9>();
    while (*in.p <= ' ') ++in.p;
    for (usize i = 0; i < n; ++i) g[i] = in.read<u32, 9>();
    auto h = set_compose(std::span<const u32>(f), std::span<const u32>(g));
    out.write(std::span(h.p, h.n), ' ');
}
