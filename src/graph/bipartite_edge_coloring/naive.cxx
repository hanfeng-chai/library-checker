#include <toy/bipartite_coloring.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 l = in.read<u32, 6>(), r = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<std::array<u32, 2>> edges(m);
    read_pairs6(in, m, [&](u32 i, u32 u, u32 v) { edges[i] = {u, v}; });
    BipartiteColoring<true> answer(l, r, edges);
    write6(out, answer.degree);
    write_bulk6(out, std::span(answer.color.p, answer.color.n), '\n');
}
