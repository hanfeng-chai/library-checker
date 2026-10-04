#include <toy/io.h>
#include <toy/substring_lcs.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 q = in.read<u32, 6>();
    auto s = in.token(), t = in.token();
    Buffer<u64> queries(q);
    for (u32 i = 0; i < q; ++i) {
        u32 a = in.read<u32, 4>(), b = in.read<u32, 4>(), c = in.read<u32, 4>();
        queries[i] = (u64(a) << 39) | (u64(b) << 29) | (u64(c) << 19) | i;
    }
    auto answer = prefix_substring_lcs(s, t, std::move(queries));
    out.write(std::span(answer.p, answer.n));
}
