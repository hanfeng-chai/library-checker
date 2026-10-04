#include <toy/clique.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>(), m = in.read<u32, 3>();
    Buffer<u32> w(n);
    read_bulk<u32, 9>(in, std::span(w.p, w.n));
    Buffer<u128> g(n);
    std::fill(g.p, g.p + n, 0);
    for (u32 i = 0; i < m; ++i) {
        u32 u = in.read<u32, 3>(), v = in.read<u32, 3>();
        g[u] |= u128(1) << v;
        g[v] |= u128(1) << u;
    }
    out.write(clique_sum(std::span<const u128>(g), std::span<const u32>(w)));
}
