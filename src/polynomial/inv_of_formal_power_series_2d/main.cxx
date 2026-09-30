#include <toy/io.h>
#include <toy/fps_2d.h>
using namespace toy;
int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<u32> f(n * m);
    for (usize i = 0; i < f.n; ++i) f[i] = in.read<u32, 9>();
    auto g = fps_inv_2d(span<const u32>(f), n, m);
    out.write(span(g.p, g.n), ' ');
}
