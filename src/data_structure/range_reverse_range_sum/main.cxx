#include <toy/io.h>
#include <toy/rope.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), q = in.read<u32, 6>();
    Buffer<u32> a(n);
    for (auto &x : std::span(a.p, a.n)) x = in.read<u32, 10>();
    if (!n && q)
        while (*in.p <= ' ') ++in.p;
    Rope sequence{std::span<const u32>(a)};
    while (q--) {
        u32 type = in.read<u32, 1>();
        auto [l, r] = in.read_pair<6>();
        if (!type)
            sequence.reverse(l, r);
        else
            out.write(sequence.sum(l, r));
    }
}
