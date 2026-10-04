#include <toy/io.h>
#include <toy/tree_isomorphism.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>();
    Buffer<u32> p(n);
    p[0] = 0;
    for (u32 i = 1; i < n; ++i) p[i] = in.read<u32, 6>();
    auto result = rooted_classes(std::span<const u32>(p));
    out.write(result.count);
    out.write(std::span(result.color.p, result.color.n));
}
