#include <toy/incremental_scc.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<u32> weight(n);
    read_bulk9(in, std::span(weight.p, weight.n));
    Buffer<std::array<u32, 2>> edges(m);
    read_pairs6(in, m, [&](u32 i, u32 u, u32 v) { edges[i] = {u, v}; });
    auto answer = incremental_scc_sum(std::span<const u32>(weight),
                                      std::span<const std::array<u32, 2>>(edges));
    out.write(std::span(answer.p, answer.n));
}
