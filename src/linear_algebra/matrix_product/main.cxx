#include <toy/io_batch.h>
#include <toy/matrix_product.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 4>(), m = in.read<u32, 4>(), k = in.read<u32, 4>();
    Buffer<u32> a(n * m), b(m * k);
    read_bulk9(in, std::span(a.p, a.n));
    read_bulk9(in, std::span(b.p, b.n));
    auto c = matrix_product(n, m, k, std::span(a.p, a.n), std::span(b.p, b.n));
    write_bulk9(out, std::span<const u32>(c.p, c.n));
    out.put('\n');
}
