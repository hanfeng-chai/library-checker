#include <toy/io.h>
#include <toy/polynomial_factorization.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 3>();
    u32 p = in.read<u32, 9>();
    Buffer<u32> f(n + 1);
    for (usize i = 0; i <= n; ++i) f[i] = in.read<u32, 9>();
    auto factors = polynomial_factorize(std::span<const u32>(f), p);
    out.write(factors.n);
    for (usize i = 0; i < factors.n; ++i) {
        auto &[a, e] = factors.data[i];
        out.write(e, ' ');
        out.write(a.n - 1, ' ');
        out.write(std::span(a.p, a.n), ' ');
        out.put('\n');
    }
}
