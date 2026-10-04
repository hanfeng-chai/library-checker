#include <toy/io.h>
#include <toy/polynomial.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>(), m = in.read<u32, 6>();
    u32 c = in.read<u32, 9>();
    Buffer<u32> values(n);
    for (usize i = 0; i < n; ++i) values[i] = in.read<u32, 9>();
    auto shifted = polynomial_shift_samples(std::span<const u32>(values), m, c);
    out.write(std::span(shifted.p, shifted.n), ' ');
}
