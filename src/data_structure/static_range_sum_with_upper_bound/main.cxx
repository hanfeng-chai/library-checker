#include <toy/io.h>
#include <toy/range_sum_bound.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32, 6>(), q = in.read<u32, 6>(); Buffer<u32> a(n);
    for (auto& x : span(a.p, a.n)) x = in.read<u32, 10>();
    Buffer<BoundedSumQuery> queries(q);
    for (u32 i = 0; i < q; ++i) { auto [l, r] = in.read_pair<6>(); queries[i] = {l, r, in.read<u32, 10>(), i}; }
    auto answers = range_sum_bound(span<const u32>(a), std::move(queries));
    for (auto x : span(answers.p, answers.n)) { out.write(x.count, ' '); out.write(x.sum); }
}
