#include <toy/distinct.h>
#include <toy/io.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>(), q = in.read<u32, 6>();
    Buffer<u32> a(n);
    for (auto &x : std::span(a.p, a.n)) x = in.read<u32, 10>();
    if (!n && q)
        while (*in.p <= ' ') ++in.p;
    Buffer<RangeQuery> queries(q);
    for (auto &query : std::span(queries.p, queries.n)) {
        auto [l, r] = in.read_pair<6>();
        query = {l, r};
    }
    auto answers = range_distinct(std::span<const u32>(a), std::span<const RangeQuery>(queries));
    out.write(std::span(answers.p, answers.n));
}
