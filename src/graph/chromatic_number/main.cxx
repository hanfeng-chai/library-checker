#include <toy/chromatic.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 2>(), m = in.read<u32, 3>();
    Buffer<u64> g(n);
    std::fill(g.p, g.p + n, 0ull);
    for (u32 i = 0; i < m; ++i) {
        u32 u = in.read<u32, 2>(), v = in.read<u32, 2>();
        g[u] |= 1ull << v;
        g[v] |= 1ull << u;
    }
    out.write(chromatic_number(g));
}
