#include <toy/io.h>
#include <toy/multiplicative_convolution.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize p = in.read<u32, 6>();
    Buffer<u32> a(p), b(p);
    for (usize i = 0; i < p; ++i) a[i] = in.read<u32, 9>();
    for (usize i = 0; i < p; ++i) b[i] = in.read<u32, 9>();
    auto c = multiplicative_convolution_prime(a, b);
    out.write(span(c.p, c.n), ' ');
}
