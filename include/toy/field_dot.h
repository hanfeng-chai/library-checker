#pragma once
#include <toy/mod.h>
namespace toy {
// a is canonical, b is Montgomery encoded. Both spans are readable up to the
// next multiple of eight, and b's padded entries are zero. Integer arithmetic
// stays exact: fold high words every eight vectors, then reduce once per lane.
template <u32 P = 998244353>
u32 field_dot_padded(const u32 *a, const u32 *b, u32 n) {
    using M = Mod<P>;
    u32 length = (n + 7) & ~7u;
    auto even = _mm256_setzero_si256(), odd = even, bound = _mm256_set1_epi64x(u64(2 * P) << 32);
    for (u32 first = 0; first < length; first += 64) {
        for (u32 i = first; i < std::min(first + 64, length); i += 8) {
            auto x = _mm256_loadu_si256((const __m256i *)(a + i)),
                 y = _mm256_loadu_si256((const __m256i *)(b + i));
            even = _mm256_add_epi64(even, _mm256_mul_epu32(x, y));
            odd = _mm256_add_epi64(
                odd, _mm256_mul_epu32(_mm256_srli_epi64(x, 32), _mm256_srli_epi64(y, 32)));
        }
        even = _mm256_min_epu32(even, _mm256_sub_epi32(even, bound));
        odd = _mm256_min_epu32(odd, _mm256_sub_epi32(odd, bound));
    }
    auto value = M::mont_sum8(even, odd);
    auto sum = _mm256_add_epi64(_mm256_cvtepu32_epi64(_mm256_castsi256_si128(value)),
                                _mm256_cvtepu32_epi64(_mm256_extracti128_si256(value, 1)));
    auto pair = _mm_add_epi64(_mm256_castsi256_si128(sum), _mm256_extracti128_si256(sum, 1));
    return (u64(_mm_cvtsi128_si64(pair)) + u64(_mm_extract_epi64(pair, 1))) % P;
}
} // namespace toy
