#include <toy/io.h>
#include <toy/min_plus_convolution.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<i32> a(n), b(m);
    for (usize i = 0; i < n; ++i) a[i] = in.read<u64, 10>();
    for (usize i = 0; i < m; ++i) b[i] = in.read<u64, 10>();
    auto c = min_plus_convex_convex<i32>(a, b);
    out.write(std::span(c.p, c.n), ' ');
}
