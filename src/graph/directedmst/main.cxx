#include <toy/directed_mst.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>(), s = in.read<u32, 6>();
    DirectedMST g(n, m);
    for (u32 i = 0; i < m; ++i) {
        u32 u = in.read<u32, 6>(), v = in.read<u32, 6>(), w = in.read<u32, 10>();
        g.add(u, v, w);
    }
    auto answer = g.solve(s);
    out.write(answer.cost);
    write_bulk6(out, std::span(answer.parent.p, answer.parent.n));
    out.put('\n');
}
