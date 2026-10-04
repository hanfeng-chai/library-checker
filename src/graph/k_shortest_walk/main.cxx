#include <toy/io_batch.h>
#include <toy/k_shortest_walk.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>(), s = in.read<u32, 6>(), t = in.read<u32, 6>(),
        k = in.read<u32, 6>();
    Buffer<std::array<u32, 3>> edges(m);
    read_triples(in, m, [&](u32 i, u32 u, u32 v, u32 w) { edges[i] = {u, v, w}; });
    auto answer = k_shortest_walk(n, edges, s, t, k);
    out.write(std::span(answer.p, answer.n));
}
