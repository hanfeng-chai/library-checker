#include <toy/cartesian.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 7>();
    Buffer<u32> a(n);
    for (auto &x : std::span(a.p, a.n)) x = in.read<u32, 10>();
    auto p = cartesian_tree(std::span<const u32>(a));
    out.write(std::span(p.p, p.n));
}
