#include <toy/io.h>
using namespace toy;

u64 prefix[500001];

int main() {
    Reader in;
    Writer out;
    int n = in.read<u32, 6>(), q = in.read<u32, 6>();
    for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + in.read<u64, 10>();
    constexpr int B = 128;
    u32 left[B], right[B];
    u64 answers[B];
    while (q) {
        int count = std::min(q, B);
        // Prefetch both endpoints before the separate lookup loop.
        for (int i = 0; i < count; ++i) {
            auto [l, r] = in.read_pair<6>();
            left[i] = l;
            right[i] = r;
            __builtin_prefetch(prefix + left[i]);
            __builtin_prefetch(prefix + right[i]);
        }
        for (int i = 0; i < count; ++i) answers[i] = prefix[right[i]] - prefix[left[i]];
        out.write(std::span(answers, count));
        q -= count;
    }
}
