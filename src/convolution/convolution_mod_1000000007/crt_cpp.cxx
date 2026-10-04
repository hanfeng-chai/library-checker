#define TOY_NTT_ASM 0
#include <toy/convolution_crt_fast.h>
#include <toy/io_batch.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<u32> a(n), b(m);
    read_bulk<u32, 10>(in, std::span(a.p, a.n));
    read_bulk<u32, 10>(in, std::span(b.p, b.n));
    auto c = convolution_crt_fast<1000000007>(a, b);
    out.write(std::span(c.p, c.n), ' ');
}
