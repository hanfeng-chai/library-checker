#include <toy/io.h>
#include <toy/divisor_convolution.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 7>();
    Buffer<u32> a(n + 1), b(n + 1);
    a[0] = b[0] = 0;
    for (usize i = 1; i <= n; ++i) a[i] = in.read<u32, 9>();
    for (usize i = 1; i <= n; ++i) b[i] = in.read<u32, 9>();
    auto c = divisor_convolution<Divisor::Lcm>(std::move(a), std::move(b));
    out.write(span(c.p + 1, n), ' ');
}
