#include <toy/io_batch.h>
#include <toy/sparse_determinant.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 4>(), m = in.read<u32, 5>();
    Buffer<MatrixEntry> a(m);
    read_triples(in, m, [&](u32 i, u32 r, u32 c, u32 x) { a[i] = {r, c, x}; });
    out.write(sparse_determinant(n, std::span<const MatrixEntry>(a.p, a.n)));
}
