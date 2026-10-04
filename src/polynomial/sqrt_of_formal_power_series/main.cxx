#include <toy/fps.h>
#include <toy/io.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>();
    Buffer<u32> f(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    auto g = fps_sqrt(std::span<const u32>(f), n);
    if (!g)
        out.write(-1);
    else
        out.write(std::span(g->p, g->n), ' ');
}
