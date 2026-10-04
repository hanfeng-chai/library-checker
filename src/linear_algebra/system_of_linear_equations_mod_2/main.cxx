#include <toy/bit_matrix.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 4>(), m = in.read<u32, 4>();
    BitMatrix a(n, m + 1);
    for (u32 i = 0; i < n; ++i) pack_bits(a[i], in.token());
    auto rhs = in.token();
    for (u32 i = 0; i < n; ++i)
        if (rhs[i] == '1') a.set(i, m);
    u32 rank = a.eliminate(m, true);
    for (u32 i = rank; i < n; ++i)
        if (a.get(i, m)) {
            out.write(-1);
            return 0;
        }
    out.write(m - rank);
    Buffer<char> text(m);
    std::fill(text.p, text.p + m, '0');
    Buffer<u8> used(m);
    std::fill(used.p, used.p + m, 0);
    for (u32 i = 0; i < rank; ++i) {
        used[a.pivot[i]] = 1;
        text[a.pivot[i]] = '0' + a.get(i, m);
    }
    out.append({text.p, m});
    out.put('\n');
    for (u32 j = 0; j < m; ++j)
        if (!used[j]) {
            std::fill(text.p, text.p + m, '0');
            text[j] = '1';
            for (u32 i = 0; i < rank; ++i) text[a.pivot[i]] = '0' + a.get(i, j);
            out.append({text.p, m});
            out.put('\n');
        }
}
