#include <toy/io_batch.h>
#include <toy/minimum_diameter_tree.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 4>(), m = in.read<u32, 4>();
    Buffer<std::array<u32, 3>> edges(m);
    read_triples(in, m, [&](u32 i, u32 u, u32 v, u32 w) { edges[i] = {u, v, w}; });
    auto answer = minimum_diameter_tree(n, edges);
    out.write(answer.diameter);
    write_bulk6(out, std::span(answer.edge.p, answer.edge.n));
    out.put('\n');
}
