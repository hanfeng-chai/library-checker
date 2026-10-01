#include <toy/io.h>
#include <toy/polynomial.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>();
    u32 a = in.read<u32, 9>(), r = in.read<u32, 9>();
    Buffer<u32> f(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    auto g = polynomial_eval_geometric(std::span<const u32>(f), m, a, r);
    out.write(std::span(g.p, g.n), ' ');
}
