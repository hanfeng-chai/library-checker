#include <toy/io.h>
#include <toy/bitwise_convolution.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = usize(1) << in.read<u32, 2>();
    Buffer<u32> a(n), b(n);
    for (usize i = 0; i < n; ++i) a[i] = in.read<u32, 9>();
    for (usize i = 0; i < n; ++i) b[i] = in.read<u32, 9>();
    auto c = bitwise_convolution<Bitwise::And>(move(a), move(b));
    out.write(span(c.p, c.n), ' ');
}
