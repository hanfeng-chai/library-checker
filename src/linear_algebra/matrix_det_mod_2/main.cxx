#include <toy/bit_matrix.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 4>();
    BitMatrix a(n, n);
    for (u32 i = 0; i < n; ++i) pack_bits(a[i], in.token());
    out.write(u32(a.eliminate(n) == n));
}
