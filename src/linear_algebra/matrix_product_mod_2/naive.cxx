#include <toy/bit_matrix.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 4>(), m = in.read<u32, 4>(), k = in.read<u32, 4>();
    BitMatrix a(n, m), b(m, k);
    for (u32 i = 0; i < n; ++i) pack_bits(a[i], in.token());
    for (u32 i = 0; i < m; ++i) pack_bits(b[i], in.token());
    auto c = bit_matrix_product(a, b);
    Buffer<char> text(k);
    for (u32 i = 0; i < n; ++i) {
        unpack_bits(c[i], 0, text);
        out.append({text.p, k});
        out.put('\n');
    }
}
