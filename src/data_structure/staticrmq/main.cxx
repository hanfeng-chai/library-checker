#include <toy/io.h>
#include <toy/rmq.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32, 6>(), q = in.read<u32, 6>(); Buffer<u32> a(n);
    for (auto& x : std::span(a.p, a.n)) x = in.read<u32, 10>();
    RMQ tree(std::move(a)); std::array<u32, 2> queries[128]; u32 answers[128];
    while (q) {
        usize count = std::min<usize>(q, 128);
        for (usize i = 0; i < count; ++i) {
            auto [l, r] = queries[i] = in.read_pair<6>();
            __builtin_prefetch(tree.suffix.p + l); __builtin_prefetch(tree.prefix.p + r - 1);
        }
        for (usize i = 0; i < count; ++i) answers[i] = tree.min(queries[i][0], queries[i][1]);
        out.write(std::span(answers, count)); q -= count;
    }
}
