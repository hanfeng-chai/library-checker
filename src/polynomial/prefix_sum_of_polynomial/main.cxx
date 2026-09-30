#include <toy/io.h>
#include <toy/polynomial.h>
using namespace toy;
int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>();
    Buffer<u32> f(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    auto g = polynomial_prefix_sum(std::move(f));
    out.write(span(g.p, g.n), ' ');
}
