#include <toy/incremental_mst.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 7>();
    IncrementalMST forest(n, m);
    read_triples(in, m,
                 [&](u32 id, u32 u, u32 v, u32 w) { out.write(forest.add(u, v, w, id), ' '); });
    out.put('\n');
}
