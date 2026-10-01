#include <toy/io.h>
#include <toy/multipoint.h>
using namespace toy;
int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 6>();
    Buffer<u32> points(n), values(n);
    for (usize i = 0; i < n; ++i) points[i] = in.read<u32, 9>();
    for (usize i = 0; i < n; ++i) values[i] = in.read<u32, 9>();
    Multipoint tree{std::span<const u32>(points)};
    auto f = tree.interpolate(std::span<const u32>(values));
    out.write(std::span(f.p, f.n), ' ');
}
