#include <toy/io.h>
#include <toy/fps_sparse.h>
using namespace toy;

int main() {
    Reader in; Writer out;
    usize n = in.read<u32, 7>(), k = in.read<u32, 2>();
    u64 exponent = in.read<u64, 19>();
    Buffer<SparseTerm> terms(k);
    for (usize i = 0; i < k; ++i) terms[i] = {in.read<u32, 6>(), in.read<u32, 9>()};
    auto g = fps_pow_sparse(span<const SparseTerm>(terms), n, exponent);
    out.write(span(g.p, g.n), ' ');
}
