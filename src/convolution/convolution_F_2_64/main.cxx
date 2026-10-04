#include <toy/convolution_gf64.h>
#include <toy/io.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>(), size = std::bit_ceil(n + m - 1);
    Buffer<u64> a(n, size), b(m, size);
    for (usize i = 0; i < n; ++i) a[i] = in.read<u64>();
    for (usize i = 0; i < m; ++i) b[i] = in.read<u64>();
    auto c = convolution_gf64(std::move(a), std::move(b));
    out.write(std::span(c.p, c.n), ' ');
}
