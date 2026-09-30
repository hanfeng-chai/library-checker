#include <toy/io.h>
#include <toy/set_series.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = usize(1) << in.read<u32, 2>(), m = in.read<u32, 6>(); Buffer<u32> f(n), w(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    for (usize i = 0; i < n; ++i) w[i] = in.read<u32, 9>();
    auto h = set_power_projection(span<const u32>(f), span<const u32>(w), m); out.write(span(h.p, h.n), ' '); out.put('\n');
}
