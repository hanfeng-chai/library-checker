#pragma GCC optimize("unroll-loops")
#include <toy/io_batch.h>
#include <toy/kruskal_workspace.h>
using namespace toy;
alignas(64) MSTEdge edges[500000], scratch[500000];
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>();
    auto *rows = ::new (static_cast<void *>(scratch)) std::array<u32, 3>[m];
    read_triple_rows(in, std::span(rows, m));
    for (u32 i = 0; i < m; ++i) edges[i] = {rows[i][0], rows[i][1], rows[i][2], i};
    auto *work = ::new (static_cast<void *>(scratch)) MSTEdge[500000];
    if (m + 1 == n) {
        u64 sum = 0;
        for (u32 i = 0; i < m; ++i) sum += edges[i].weight;
        out.write(sum);
        for (u32 i = 0; i < m; ++i) write6(out, i, ' ');
        out.put('\n');
        return 0;
    }
    auto answer = kruskal_workspace(n, std::span(edges, m), std::span(work, 500000));
    out.write(answer.cost);
    write_bulk6(out, answer.edge);
    out.put('\n');
}
