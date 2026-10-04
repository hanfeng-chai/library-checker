#include <toy/chordal.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<std::array<u32, 2>> edges(m);
    read_pairs6(in, m, [&](u32 i, u32 u, u32 v) { edges[i] = {u, v}; });
    Adjacency g(n, edges);
    edges = {};
    ChordalGraph answer(g);
    if (answer.chordal) {
        out.append("YES\n");
        write_bulk6(out, std::span(answer.order.p, answer.order.n));
    } else {
        out.append("NO\n");
        write6(out, answer.cycle.n);
        write_bulk6(out, std::span(answer.cycle.p, answer.cycle.n));
    }
    out.put('\n');
}
