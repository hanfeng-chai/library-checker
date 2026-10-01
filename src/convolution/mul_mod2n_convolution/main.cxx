#include <toy/io.h>
#include <toy/multiplicative_convolution.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = usize(1) << in.read<u32, 2>();
    Buffer<u32> a(n), b(n);
    for (usize i = 0; i < n; ++i) a[i] = in.read<u32, 9>();
    for (usize i = 0; i < n; ++i) b[i] = in.read<u32, 9>();
    auto c = multiplicative_convolution_2n(a, b);
    out.write(std::span(c.p, c.n), ' ');
}
