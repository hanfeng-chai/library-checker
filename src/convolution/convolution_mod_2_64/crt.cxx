#define TOY_CRT_FUSED_NTT 0
#define TOY_NTT_ASM 0
#include <toy/convolution_u64.h>
#include <toy/io_batch.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<u64> a(n), b(m);
    read_bulk<u64, 20>(in, std::span(a.p, a.n));
    read_bulk<u64, 20>(in, std::span(b.p, b.n));
    auto c = convolution_u64(a, b);
    out.write(std::span(c.p, c.n), ' ');
}
