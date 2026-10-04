#include <toy/io.hpp>
#include <toy/parallel_dsu.hpp>
int main() {
    constexpr toy::u32 mod = 998244353;
    toy::Reader in(toy::direct_mapping);
    toy::Writer out;
    int n = in.read_uniform<6, int>(), q = in.read_uniform<6, int>();
    std::vector<toy::u32> x(n);
    for (auto &v : x) v = in.read_uniform<9, toy::u32>();
    toy::ParallelUnionFind<mod> d(x);
    while (q--) {
        int k = in.read_uniform<6, int>(), a = in.read_uniform<6, int>(),
            b = in.read_uniform<6, int>(), offset = 0;
        for (int level = 0; k; k >>= 1, ++level)
            if (k & 1) {
                d.unite(level, a + offset, b + offset);
                offset += 1 << level;
            }
        out.write_padded_u32(d.value());
    }
}
