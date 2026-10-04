#include <toy/convolution_fast.h>
#include <toy/io_batch.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 8>(), m = in.read<u32, 8>();
    usize size = std::bit_ceil(n + m - 1);
    auto a = ntt_storage<u32>(n, size + 16), b = ntt_storage<u32>(m, size + 16);
    read_bulk9(in, std::span(a.p, a.n));
    read_bulk9(in, std::span(b.p, b.n));
    auto c = convolution_fast(std::move(a), std::move(b));
    write_bulk9(out, std::span<const u32>(c));
}
