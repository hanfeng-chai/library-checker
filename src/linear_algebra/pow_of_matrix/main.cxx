#include <toy/frobenius.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>();
    u64 k = in.read<u64, 19>();
    Buffer<u32> a(n * n);
    read_bulk9(in, std::span(a.p, a.n));
    auto r = matrix_power(n, std::span(a.p, a.n), k);
    write_bulk9(out, std::span<const u32>(r.p, r.n));
    out.put('\n');
}
