#include <toy/io_batch.h>
#include <toy/st_numbering.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 tests = in.read<u32, 6>();
    while (tests--) {
        u32 n = in.read<u32, 6>(), m = in.read<u32, 6>(), s = in.read<u32, 6>(),
            t = in.read<u32, 6>();
        Buffer<std::array<u32, 2>> edges(m);
        read_bulk<u32, 6>(in, std::span((u32 *)edges.p, 2 * m));
        Adjacency g(n, edges);
        edges = {};
        auto rank = st_numbering(g, s, t);
        if (!rank.n) {
            out.append("No\n");
            continue;
        }
        out.append("Yes\n");
        write_bulk6(out, std::span(rank.p, rank.n));
        out.put('\n');
    }
}
