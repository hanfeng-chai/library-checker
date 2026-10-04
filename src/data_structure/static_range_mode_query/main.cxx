#include <toy/io.h>
#include <toy/mode.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), q = in.read<u32, 6>();
    if (!q) return 0;
    Buffer<u32> a(n);
    for (auto &x : std::span(a.p, a.n)) x = in.read<u32, 10>();
    StaticRangeMode mode{std::span<const u32>(a), q};
    while (q--) {
        auto [l, r] = in.read_pair<6>();
        auto [value, count] = mode.query(l, r);
        out.write(value, ' ');
        out.write(count);
    }
}
