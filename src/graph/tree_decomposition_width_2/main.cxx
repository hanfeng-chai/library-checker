#include <toy/io_batch.h>
#include <toy/treewidth2.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    in.token();
    in.token();
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<std::array<u32, 2>> edges(m);
    read_pairs6(in, m, [&](u32 i, u32 u, u32 v) { edges[i] = {u - 1, v - 1}; });
    Treewidth2 answer(n, edges);
    if (!answer.exists) {
        out.write(-1);
        return 0;
    }
    out.append("s td ");
    write6(out, n, ' ');
    out.append("2 ");
    write6(out, n);
    for (u32 v = 0; v < n; ++v) {
        out.append("b ");
        write6(out, v + 1, ' ');
        auto b = answer.bag[v];
        write6(out, v + 1, b.first == ~0u ? '\n' : ' ');
        if (b.first != ~0u) write6(out, b.first + 1, b.second == ~0u ? '\n' : ' ');
        if (b.second != ~0u) write6(out, b.second + 1);
    }
    for (u32 v = 0; v < n; ++v)
        if (answer.parent[v] != v) {
            write6(out, v + 1, ' ');
            write6(out, answer.parent[v] + 1);
        }
}
