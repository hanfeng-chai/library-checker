#include <toy/io_batch.h>
#include <toy/matrix_product.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>();
    u64 k = in.read<u64, 19>();
    Buffer<u32> a(n * n), r(n * n);
    read_bulk9(in, std::span(a.p, a.n));
    std::fill(r.p, r.p + r.n, 0u);
    for (u32 i = 0; i < n; ++i) r[i * n + i] = 1;
    for (; k; k >>= 1) {
        if (k & 1) r = matrix_product(n, n, n, std::span(r.p, r.n), std::span(a.p, a.n));
        if (k > 1) a = matrix_product(n, n, n, std::span(a.p, a.n), std::span(a.p, a.n));
    }
    write_bulk9(out, std::span<const u32>(r.p, r.n));
    out.put('\n');
}
