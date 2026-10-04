#include <toy/io_batch.h>
#include <toy/weighted_matching.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>(), m = in.read<u32, 6>();
    WeightedMatching graph(n);
    read_triples(in, m, [&](u32, u32 u, u32 v, u32 w) { graph.add(u, v, w); });
    u64 value = graph.solve();
    u32 count = 0;
    for (u32 v = 1; v <= n; ++v) count += graph.mate[v] > v;
    out.write(count, ' ');
    out.write(value);
    for (u32 v = 1; v <= n; ++v)
        if (graph.mate[v] > v) {
            write6(out, v - 1, ' ');
            write6(out, graph.mate[v] - 1);
        }
}
