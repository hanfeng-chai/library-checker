#include <toy/io.h>
#include <toy/polynomial_gcd.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 5>(), m = in.read<u32, 5>();
    Buffer<u32> f(n), g(m);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    for (usize i = 0; i < m; ++i) g[i] = in.read<u32, 9>();
    auto h = polynomial_inv_mod(std::span<const u32>(f), std::span<const u32>(g));
    if (!h) {
        out.write(-1);
        return 0;
    }
    out.write(h->n);
    out.write(std::span(h->p, h->n), ' ');
    out.put('\n');
}
