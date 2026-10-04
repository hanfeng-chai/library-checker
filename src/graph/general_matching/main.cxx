#include <toy/general_matching.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>(), m = in.read<u32, 6>();
    Buffer<std::array<u32, 2>> edges(m);
    read_pairs6(in, m, [&](u32 i, u32 u, u32 v) { edges[i] = {u, v}; });
    GeneralMatching answer(n, edges);
    out.write(answer.size);
    for (u32 u = 1; u <= n; ++u)
        if (answer.mate[u] > u) {
            write6(out, u - 1, ' ');
            write6(out, answer.mate[u] - 1);
        }
}
