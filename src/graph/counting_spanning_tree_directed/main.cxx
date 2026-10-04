#include <toy/io_batch.h>
#include <toy/spanning_count.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>(), m = in.read<u32, 6>();
    u32 root = in.read<u32, 3>();
    SpanningCount laplace(n, root);
    read_pairs6(in, m, [&](u32, u32 u, u32 v) { laplace.add(u, v); });
    out.write(laplace.count());
}
