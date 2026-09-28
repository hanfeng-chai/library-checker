#include <toy/io.hpp>

#ifndef TOY_BLOCK_LOG
#define TOY_BLOCK_LOG 11
#endif

int main() {
    toy::Reader in(toy::direct_mapping);
    toy::Writer out;
    int t = in.read_uniform<7, toy::u32>();
    constexpr int block_size = 1 << TOY_BLOCK_LOG;
    alignas(64) toy::u64 sums[block_size];
    while (t) {
        int count = std::min(t, block_size);
        for (int i = 0; i < count; ++i) {
            toy::u64 a = in.read_var<19, toy::u64>();
            toy::u64 b = in.read_var<19, toy::u64>();
            sums[i] = a + b;
        }
        for (int i = 0; i < count; ++i)
            out.write_token_bounded<2'000'000'000'000'000'000ULL>(sums[i]);
        t -= count;
    }
}