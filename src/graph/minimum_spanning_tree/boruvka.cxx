#include <toy/io_batch.h>
#include <toy/spanning_tree.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<std::array<u32, 3>> edges(m);
    read_triple_rows(in, std::span(edges.p, edges.n));
    SpanningTree tree;
    if (m + 1 == n) {
        tree.edge = Buffer<u32>(m);
        std::iota(tree.edge.p, tree.edge.p + m, 0u);
        for (auto e : std::span(edges.p, edges.n)) tree.cost += e[2];
    } else if (u64(n) * n <= 4ull * m)
        tree = dense_prim(n, edges);
    else
        tree = boruvka(n, edges);
    out.write(tree.cost);
    write_bulk6(out, std::span(tree.edge.p, tree.edge.n));
    out.put('\n');
}
