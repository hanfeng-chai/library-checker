#include <toy/io.h>
#include <toy/wavelet.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>(), q = in.read<u32, 6>();
    Buffer<u32> a(n);
    for (auto &x : std::span(a.p, a.n)) x = in.read<u32, 10>();
    WaveletMatrix matrix{std::span<const u32>(a)};
    while (q--) {
        auto [l, r] = in.read_pair<6>();
        u32 k = in.read<u32, 6>();
        out.write(matrix.kth(l, r, k));
    }
}
