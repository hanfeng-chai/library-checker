#include <toy/io.h>
#include <toy/multivariate_convolution.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize k = in.read<u32, 2>(), n = 1;
    Buffer<u32> dimensions(k);
    for (usize i = 0; i < k; ++i) n *= dimensions[i] = in.read<u32, 6>();
    while (*in.p <= ' ') ++in.p; // K=0 has an empty dimension line.
    Buffer<u32> a(n), b(n);
    for (usize i = 0; i < n; ++i) a[i] = in.read<u32, 9>();
    for (usize i = 0; i < n; ++i) b[i] = in.read<u32, 9>();
    auto c = multivariate_convolution(span<const u32>(dimensions), std::move(a), std::move(b));
    out.write(span(c.p, c.n), ' ');
}
