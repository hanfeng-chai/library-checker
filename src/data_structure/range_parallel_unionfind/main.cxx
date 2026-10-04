#include <toy/io.h>
#include <toy/mod.h>
#include <toy/parallel_dsu.h>
using namespace toy;
int main() {
    using M = Mod<998244353>;
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), q = in.read<u32, 6>(), answer = 0;
    Buffer<u32> sums(n);
    for (auto &x : std::span(sums.p, sums.n)) x = in.read<u32, 16>();
    ParallelDSU<3> dsu(n);
    while (q--) {
        u32 length = in.read<u32, 6>();
        auto [a, b] = in.read_pair<6>();
        dsu.unite(a, b, length, [&](u32 root, u32 removed) {
            answer = (answer + u64(sums[root]) * sums[removed]) % 998244353;
            sums[root] = M::add(sums[root], sums[removed]);
        });
        out.write(answer);
    }
}
