#include <toy/io.h>
#include <toy/polynomial_roots.h>
using namespace toy;
int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 4>(); Buffer<u32> f(n + 1);
    for (usize i = 0; i <= n; ++i) f[i] = in.read<u32, 9>();
    auto roots = polynomial_roots(std::move(f));
    out.write(roots.n); out.write(span(roots.p, roots.n), ' '); out.put('\n');
}
