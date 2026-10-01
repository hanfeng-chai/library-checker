#include <toy/io.h>
#include <toy/convolution.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 8>(), m = in.read<u32, 8>();
    usize size = std::bit_ceil(n + m - 1);
    Buffer<u32> a(n, size), b(m, size);
    for (usize i = 0; i < n; ++i) a[i] = in.read<u32, 9>();
    for (usize i = 0; i < m; ++i) b[i] = in.read<u32, 9>();
    auto c = convolution(std::move(a), std::move(b));
    out.write(std::span(c.p, c.n), ' ');
}
