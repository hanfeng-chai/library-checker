#include <toy/io.h>
#include <toy/fps_composition.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32, 6>(); Buffer<u32> f(n), g(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    for (usize i = 0; i < n; ++i) g[i] = in.read<u32, 9>();
    auto h = fps_compose(std::span<const u32>(f), std::span<const u32>(g), n);
    out.write(std::span(h.p, h.n), ' ');
}
