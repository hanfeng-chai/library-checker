#include <toy/io.h>
#include <toy/multipoint.h>
using namespace toy;
int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>();
    Buffer<u32> f(n), points(n);
    for (usize i = 0; i < n; ++i) f[i] = in.read<u32, 9>();
    for (usize i = 0; i < n; ++i) points[i] = in.read<u32, 9>();
    Multipoint tree{span<const u32>(points)};
    auto result = tree.to_newton(std::move(f));
    out.write(span(result.p, result.n), ' ');
}
