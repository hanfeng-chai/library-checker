#include <toy/bipartite_matching.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 l = in.read<u32, 6>(), r = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<std::array<u32, 2>> edges(m);
    read_bulk<u32, 5>(in, std::span((u32 *)edges.p, 2 * m));
    BipartiteMatching match(l, r, std::move(edges));
    write6(out, match.size);
    for (u32 u = 0; u < l; ++u)
        if (match.left[u] != ~0u) {
            write6(out, u, ' ');
            write6(out, match.left[u]);
        }
}
