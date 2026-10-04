#include <toy/io_batch.h>
#include <toy/steiner_tree.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>(), m = in.read<u32, 4>();
    Buffer<std::array<u32, 3>> edges(m);
    read_triples(in, m, [&](u32 i, u32 u, u32 v, u32 w) { edges[i] = {u, v, w}; });
    u32 k = in.read<u32, 2>();
    Buffer<u32> terminals(k);
    read_bulk<u32, 2>(in, std::span(terminals.p, terminals.n));
    auto answer = steiner_tree(n, edges, terminals);
    out.write(answer.cost, ' ');
    write6(out, answer.edge.n);
    write_bulk6(out, std::span(answer.edge.p, answer.edge.n));
    out.put('\n');
}
