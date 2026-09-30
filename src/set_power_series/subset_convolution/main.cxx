#include <toy/io.h>
#include <toy/subset_convolution.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = usize(1) << in.read<u32, 2>(); Buffer<u32> f(n), g(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    for (usize i = 0; i < n; ++i) g[i] = in.read<u32, 9>();
    auto h = subset_convolution(std::move(f), std::move(g)); out.write(span(h.p, h.n), ' ');
}
