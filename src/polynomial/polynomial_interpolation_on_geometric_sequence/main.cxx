#include <toy/io.h>
#include <toy/polynomial.h>
using namespace toy;

int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>();
    u32 a = in.read<u32, 9>(), r = in.read<u32, 9>();
    Buffer<u32> values(n);
    for (usize i = 0; i < n; ++i) values[i] = in.read<u32, 9>();
    auto f = polynomial_interpolate_geometric(std::move(values), a, r);
    out.write(std::span(f.p, f.n), ' ');
}
