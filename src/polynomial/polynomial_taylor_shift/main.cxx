#include <toy/io.h>
#include <toy/polynomial.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>();
    u32 c = in.read<u32, 9>();
    Buffer<u32> f(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    auto g = polynomial_taylor_shift(std::move(f), c);
    out.write(std::span(g.p, g.n), ' ');
}
