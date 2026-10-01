#include <toy/io.h>
#include <toy/set_series.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = usize(1) << in.read<u32, 2>(); Buffer<u32> f(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    auto g = set_log(std::span<const u32>(f)); out.write(std::span(g.p, g.n), ' ');
}
