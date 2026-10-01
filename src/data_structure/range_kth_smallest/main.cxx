#include <toy/io.h>
#include <toy/offline_kth.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32, 6>(), q = in.read<u32, 6>(); Buffer<u32> a(n);
    for (auto& x : std::span(a.p, a.n)) x = in.read<u32, 10>();
    Buffer<u32> left(q), right(q), index(q);
    for (usize i = 0; i < q; ++i) { auto [l, r] = in.read_pair<6>(); left[i] = l; right[i] = r; index[i] = in.read<u32, 6>(); }
    auto answers = range_kth(std::span<const u32>(a), std::move(left), std::move(right), std::move(index));
    out.write(std::span(answers.p, answers.n));
}
