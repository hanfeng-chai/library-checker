#pragma once
#include <toy/mod.h>
namespace toy {
// Redundant Montgomery residues [0,2P), with P < 2^30.
template <u32 P = 998244353>
struct Montgomery {
    using M = Mod<P>;
    static constexpr u32 one = (u64(1) << 32) % P;
    static u32 multiply(u32 x, u32 y) {
        u64 z = u64(x) * y;
        return (z + u64(u32(z) * M::inverse) * P) >> 32;
    }
    static __m256i multiply(__m256i x, __m256i y) {
        auto inv = _mm256_set1_epi32(M::inverse), mod = _mm256_set1_epi32(P);
        auto even = _mm256_mul_epu32(x, y),
             odd = _mm256_mul_epu32(_mm256_srli_epi64(x, 32), _mm256_srli_epi64(y, 32));
        even = _mm256_add_epi64(even, _mm256_mul_epu32(_mm256_mul_epu32(even, inv), mod));
        odd = _mm256_add_epi64(odd, _mm256_mul_epu32(_mm256_mul_epu32(odd, inv), mod));
        return _mm256_or_si256(_mm256_srli_epi64(even, 32), odd);
    }
    static __m256i add(__m256i x, __m256i y) {
        auto z = _mm256_add_epi32(x, y);
        return _mm256_min_epu32(z, _mm256_sub_epi32(z, _mm256_set1_epi32(2 * P)));
    }
    static u32 add(u32 x, u32 y) { return std::min(x + y, x + y - 2 * P); }
    static u32 subtract(u32 x, u32 y) { return std::min(x - y, x - y + 2 * P); }
    static u32 encode(u32 x) { return multiply(x, M::r2); }
    static u32 decode(u32 x) {
        u32 y = multiply(x, 1);
        return std::min(y, y - P);
    }
    static u32 power(u32 x, u32 n) {
        u32 y = one;
        for (; n; n >>= 1, x = multiply(x, x))
            if (n & 1) y = multiply(y, x);
        return y;
    }
};
} // namespace toy
