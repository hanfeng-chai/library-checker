#include <toy/io.h>
#include <toy/fps.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>();
    u64 exponent = in.read<u64, 19>();
    Buffer<u32> f(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    auto g = fps_pow(span<const u32>(f), n, exponent);
    out.write(span(g.p, g.n), ' ');
}
