#pragma once
#include <toy/page_buffer.h>
#include <toy/set_series.h>
namespace toy {
// A matching plus fixed pairs decomposes into alternating cycles. Count each
// cycle from its greatest pair; a set-series exponential joins disjoint cycles.
template <u32 P = 998244353>
u32 hafnian_cycles(u32 n, std::span<const u32> a) {
    using M = Mod<P>;
    if (!n) return 1;
    u32 pairs = n / 2;
    bool constant = true;
    for (u32 i = 1; i < n && constant; ++i)
        for (u32 j = 0; j < i; ++j)
            if (a[i * n + j] != a[n]) {
                constant = false;
                break;
            }
    if (constant) {
        u32 result = M::pow(a[n], pairs);
        for (u32 i = 1; i < n; i += 2) result = M::mul(result, i);
        return result;
    }
    Buffer<u32> cycle(1u << pairs);
    std::fill(cycle.p, cycle.p + cycle.n, 0u);
    constexpr u64 limit = 8ull * P * P;
    auto sign = _mm_set1_epi64x(INT64_MIN), bound = _mm_set1_epi64x(limit),
         threshold = _mm_set1_epi64x((limit - 1) ^ u64(INT64_MIN));
    for (u32 h = pairs; h--;) {
        u32 states = 1u << h, width = 2 * h;
        cycle[states] = a[usize(2 * h) * n + 2 * h + 1];
        auto dp = page_buffer<u64>(usize(states) * width);
        std::fill(dp.p, dp.p + dp.n, 0ull);
        for (u32 i = 0; i < width; ++i)
            dp[usize(1u << (i / 2)) * width + (i ^ 1)] = a[usize(2 * h) * n + i];
        for (u32 mask = 1; mask < states; ++mask) {
            u64 close = 0;
            for (u32 present = mask; present; present &= present - 1) {
                u32 pair = std::countr_zero(present);
                for (u32 side = 0; side < 2; ++side) {
                    u32 last = 2 * pair + side, w = dp[usize(mask) * width + last] % P;
                    if (!w) continue;
                    close += u64(w) * a[usize(last) * n + 2 * h + 1];
                    if (close >= limit) close -= limit;
                    auto scale = _mm_set1_epi64x(w);
                    for (u32 missing = (states - 1) ^ mask; missing; missing &= missing - 1) {
                        u32 p = std::countr_zero(missing);
                        u64 *dst = dp.p + usize(mask | (1u << p)) * width + 2 * p;
                        // Exactly two adjacent states belong to this pair. The
                        // next pair belongs to a different subset, so use 128 bits.
                        auto values = _mm_cvtepu32_epi64(_mm_shuffle_epi32(
                            _mm_loadl_epi64((const __m128i *)(a.data() + usize(last) * n + 2 * p)),
                            0xe1));
                        auto x = _mm_add_epi64(_mm_load_si128((const __m128i *)dst),
                                               _mm_mul_epu32(values, scale));
                        auto reduce = _mm_cmpgt_epi64(_mm_xor_si128(x, sign), threshold);
                        _mm_store_si128((__m128i *)dst,
                                        _mm_sub_epi64(x, _mm_and_si128(reduce, bound)));
                    }
                }
            }
            cycle[states | mask] = close % P;
        }
    }
    u32 half = 1u << (pairs - 1);
    auto partitions = set_exp<P>(std::span<const u32>(cycle.p, half));
    u128 answer = 0;
    for (u32 mask = 0; mask < half; ++mask)
        answer += u64(partitions[mask]) * cycle[(2 * half - 1) ^ mask];
    return answer % P;
}
} // namespace toy
