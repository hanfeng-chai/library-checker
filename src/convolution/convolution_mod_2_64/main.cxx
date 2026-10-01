#include <toy/io.h>
#include <toy/convolution_crt.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<u64> a(n), b(m);
    for (usize i = 0; i < n; ++i) a[i] = in.read<u64>();
    for (usize i = 0; i < m; ++i) b[i] = in.read<u64>();
    auto c = convolution_u64(a, b);
    out.write(std::span(c.p, c.n), ' ');
}
