#include <toy/io.h>
#include <toy/fps.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>();
    Buffer<u32> f(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    auto g = fps_log(std::span<const u32>(f), n);
    out.write(std::span(g.p, g.n), ' ');
}
