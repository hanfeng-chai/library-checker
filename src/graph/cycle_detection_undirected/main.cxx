#include <toy/forest_cycle.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>();
    ForestCycle graph(n, m);
    for (u32 i = 0; i < m; ++i) {
        u32 u = in.read<u32, 6>(), v = in.read<u32, 6>();
        if (!graph.add(u, v)) continue;
        auto c = graph.cycle();
        write6(out, c.edge.n);
        write_bulk6(out, std::span(c.vertex.p, c.vertex.n));
        out.put('\n');
        write_bulk6(out, std::span(c.edge.p, c.edge.n));
        out.put('\n');
        return 0;
    }
    out.write(-1);
}
