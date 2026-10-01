#include <toy/io.h>
#include <toy/fft_convolution.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<u32> a(n), b(m);
    for (usize i = 0; i < n; ++i) a[i] = in.read<u64, 10>();
    for (usize i = 0; i < m; ++i) b[i] = in.read<u64, 10>();
    auto c = convolution_fft<1000000007>(a, b);
    out.write(std::span(c.p, c.n), ' ');
}
