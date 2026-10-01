#include <toy/io.h>
#include <toy/frequency.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32, 6>(), q = in.read<u32, 6>(); Buffer<u32> a(n); Buffer<FrequencyQuery> queries(q);
    for (auto& x : std::span(a.p, a.n)) x = in.read<u32, 10>();
    if (!n && q) while (*in.p <= ' ') ++in.p;
    for (auto& query : std::span(queries.p, queries.n)) { auto [l, r] = in.read_pair<6>(); query = {l, r, in.read<u32, 10>()}; }
    auto answers = static_frequencies(std::span<const u32>(a), std::span<const FrequencyQuery>(queries)); out.write(std::span(answers.p, answers.n));
}
