#pragma once
#include <toy/common.h>

namespace toy {

// Runtime modulus, 1 < p < 2^32. The reciprocal avoids integer division in mul.
struct Barrett {
    u32 p;
    u64 reciprocal;
    explicit Barrett(u32 p) : p(p), reciprocal(~u64(0) / p + 1) {}
    [[gnu::always_inline]] u32 mul(u32 a, u32 b) const {
        u64 x = u64(a) * b, q = (u128(x) * reciprocal) >> 64, y = q * p;
        return x - y + (x < y ? p : 0);
    }
    u32 pow(u32 a, u32 n) const {
        u32 r = 1;
        for (; n; n >>= 1, a = mul(a, a)) if (n & 1) r = mul(r, a);
        return r;
    }
};

template<u32 P>
struct Mod {
    static_assert(P > 2 && (P & 1) && P < (1u << 30));
    static constexpr u32 inverse = [] { u32 x = 2 + P; for (int i = 0; i < 4; ++i) x *= 2 + P * x; return x; }();
    static constexpr u32 r2 = -u64(P) % P;
    [[gnu::always_inline]] static constexpr u32 add(u32 a, u32 b) { return min(a + b, a + b - P); }
    [[gnu::always_inline]] static constexpr u32 sub(u32 a, u32 b) { return min(a - b, a - b + P); }
    [[gnu::always_inline]] static constexpr u32 mul(u32 a, u32 b) { return u64(a) * b % P; }
    [[gnu::always_inline]] static constexpr u32 mont(u32 a, u32 b) {
        u64 x = u64(a) * b;
        u32 y = (x + u64(u32(x) * inverse) * P) >> 32;
        return min(y, y - P);
    }
    [[gnu::always_inline]] static constexpr u32 pow(u32 a, u64 n) {
        u32 x = 1;
        for (; n; n >>= 1, a = mul(a, a)) if (n & 1) x = mul(x, a);
        return x;
    }
    [[gnu::always_inline]] static __m256i add(__m256i a, __m256i b) {
        auto x = _mm256_add_epi32(a, b);
        return _mm256_min_epu32(x, _mm256_sub_epi32(x, _mm256_set1_epi32(P)));
    }
    [[gnu::always_inline]] static __m256i sub(__m256i a, __m256i b) {
        auto x = _mm256_sub_epi32(a, b);
        return _mm256_min_epu32(x, _mm256_add_epi32(x, _mm256_set1_epi32(P)));
    }
    [[gnu::always_inline]] static __m256i mont(__m256i a, __m256i b) {
        // AVX2 widens only even u32 lanes. Reduce the two halves separately;
        // each corrected 64-bit product has zero low bits, so OR interleaves them.
        auto p = _mm256_set1_epi32(P), inv = _mm256_set1_epi32(inverse);
        auto even = _mm256_mul_epu32(a, b);
        auto odd = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32));
        even = _mm256_add_epi64(even, _mm256_mul_epu32(_mm256_mul_epu32(even, inv), p));
        odd = _mm256_add_epi64(odd, _mm256_mul_epu32(_mm256_mul_epu32(odd, inv), p));
        auto x = _mm256_or_si256(_mm256_srli_epi64(even, 32), odd);
        return _mm256_min_epu32(x, _mm256_sub_epi32(x, p));
    }
    // Reduce sums of at most eight canonical products in even/odd u64 lanes.
    // 8*(P-1)^2 + (2^32-1)*P fits u64; the quotient is below 3*P.
    [[gnu::always_inline]] static __m256i mont_sum8(__m256i even, __m256i odd) {
        auto p = _mm256_set1_epi32(P), inv = _mm256_set1_epi32(inverse);
        even = _mm256_add_epi64(even, _mm256_mul_epu32(_mm256_mul_epu32(even, inv), p));
        odd = _mm256_add_epi64(odd, _mm256_mul_epu32(_mm256_mul_epu32(odd, inv), p));
        auto x = _mm256_or_si256(_mm256_srli_epi64(even, 32), odd);
        x = _mm256_min_epu32(x, _mm256_sub_epi32(x, _mm256_set1_epi32(2 * P)));
        return _mm256_min_epu32(x, _mm256_sub_epi32(x, p));
    }
};

}
