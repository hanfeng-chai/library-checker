#include <toy/io.h>
#include <toy/polynomial.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<u32> f(n), g(m);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    for (usize i = 0; i < m; ++i) g[i] = in.read<u32, 9>();
    auto [q, r] = polynomial_divmod(std::move(f), std::span<const u32>(g));
    out.write(q.n, ' ');
    out.write(r.n);
    out.write(std::span(q.p, q.n), ' ');
    out.put('\n');
    out.write(std::span(r.p, r.n), ' ');
    out.put('\n');
}
