#include <toy/io.h>
#include <toy/linear_system.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>(), m = in.read<u32, 3>();
    Buffer<u32> a(n * (m + 1));
    for (u32 i = 0; i < n; ++i)
        for (u32 j = 0; j < m; ++j) a[i * (m + 1) + j] = in.read<u32, 9>();
    for (u32 i = 0; i < n; ++i) a[i * (m + 1) + m] = in.read<u32, 9>();
    FieldEchelon e(n, m + 1, m, std::span(a.p, a.n));
    u32 rank = e.pivot.n;
    for (u32 i = rank; i < n; ++i)
        if (e.get(i, m)) {
            out.write(-1);
            return 0;
        }
    out.write(m - rank);
    Buffer<u8> used(m);
    std::fill(used.p, used.p + m, 0);
    for (u32 p : std::span(e.pivot.p, e.pivot.n)) used[p] = 1;
    Buffer<u32> free(0, m - rank);
    for (u32 j = 0; j < m; ++j)
        if (!used[j]) free.p[free.n++] = j;
    u32 width = free.n + 1;
    Buffer<u32> rhs(rank * width);
    using M = Mod<998244353>;
    for (u32 i = 0; i < rank; ++i) {
        rhs[i * width] = e.get(i, m);
        for (u32 j = 0; j < free.n; ++j) rhs[i * width + j + 1] = M::sub(0, e.get(i, free[j]));
    }
    LazyMatrix solution(rank, width, std::span(rhs.p, rhs.n));
    for (u32 i = rank; i--;) {
        for (u32 j = 0; j < width; ++j) solution.row[i][j] %= 998244353;
        solution.count[i] = 0;
        for (u32 r = 0; r < i; ++r)
            if (u32 value = e.get(r, e.pivot[i]))
                solution.add(r, solution.row[i], nullptr, 998244353 - value, 0, 0, 0, 0);
    }
    Buffer<u32> x(m);
    for (u32 j = 0; j < width; ++j) {
        std::fill(x.p, x.p + m, 0u);
        if (j) x[free[j - 1]] = 1;
        for (u32 i = 0; i < rank; ++i) x[e.pivot[i]] = solution.get(i, j);
        out.write(std::span(x.p, x.n), ' ');
        out.put('\n');
    }
}
