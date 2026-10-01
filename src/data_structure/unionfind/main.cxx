#include <toy/io.h>
#include <toy/dsu.h>
using namespace toy;
int main() {
    Reader in; Writer out; u32 n = in.read<u32, 6>(), q = in.read<u32, 6>(); DSU dsu(n);
    struct Query { u32 type, a, b; }; Query queries[128]; char answers[256];
    while (q) {
        usize count = std::min<u32>(q, 128), used = 0;
        for (usize i = 0; i < count; ++i) {
            auto type = in.read<u32, 1>(); auto [a, b] = in.read_pair<6>(); queries[i] = {type, a, b};
            __builtin_prefetch(dsu.parent.p + a); __builtin_prefetch(dsu.parent.p + b);
        }
        for (usize i = 0; i < count; ++i) {
            auto [type, a, b] = queries[i];
            if (!type) dsu.merge(a, b);
            else { answers[used++] = '0' + dsu.same(a, b); answers[used++] = '\n'; }
        }
        out.append(std::string_view(answers, used)); q -= count;
    }
}
