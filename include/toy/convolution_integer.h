#pragma once
#include <toy/convolution.h>
#include <toy/mod.h>

namespace toy {
// Nonnegative integer convolution. Inputs are <P0 and every output is <P0*P1.
// P0<P1 are NTT primes; the default pair supports partial lengths through 2^24.
template<u32 P0 = 998244353, u32 P1 = 1004535809>
Buffer<u64> convolution_integer(span<const u32> a, span<const u32> b) {
    static_assert(P0 < P1 && P1 < (1u << 30));
    if (a.empty() || b.empty()) return {};
    usize count = a.size() + b.size() - 1;
    Buffer<u64> result(count);
    if (min(a.size(), b.size()) <= 16) {
        if (a.size() < b.size()) swap(a, b);
        for (usize k = 0; k < count; ++k) {
            u64 sum = 0;
            for (usize j = k < a.size() ? 0 : k - a.size() + 1; j < min(b.size(), k + 1); ++j) sum += u64(a[k - j]) * b[j];
            result[k] = sum;
        }
        return result;
    }
    auto under = [&]<u32 P>() {
        usize size = max<usize>(64, bit_ceil(count));
        Buffer<u32> x(a.size(), size), y(b.size(), size);
        memcpy(x.p, a.data(), a.size() * 4); memcpy(y.p, b.data(), b.size() * 4);
        return convolution<P>(std::move(x), std::move(y));
    };
    auto x = under.template operator()<P0>(), y = under.template operator()<P1>();
    using M = Mod<P1>;
    constexpr u32 inverse = M::pow(P0, P1 - 2);
    auto inv = _mm256_set1_epi32(M::mont(inverse, M::r2)), prime = _mm256_set1_epi32(P0);
    auto mask = _mm256_set1_epi64x(0xffffffff); usize i = 0;
    // CRT widens even/odd u32 lanes separately, then interleaves the u64 results.
    for (; i + 8 <= count; i += 8) {
        auto a = _mm256_load_si256((const __m256i*)(x.p + i));
        auto b = _mm256_load_si256((const __m256i*)(y.p + i));
        auto t = M::mont(M::sub(b, a), inv);
        auto even = _mm256_add_epi64(_mm256_mul_epu32(t, prime), _mm256_and_si256(a, mask));
        auto odd = _mm256_add_epi64(_mm256_mul_epu32(_mm256_srli_epi64(t, 32), prime), _mm256_srli_epi64(a, 32));
        auto lo = _mm256_unpacklo_epi64(even, odd), hi = _mm256_unpackhi_epi64(even, odd);
        _mm256_store_si256((__m256i*)(result.p + i), _mm256_permute2x128_si256(lo, hi, 0x20));
        _mm256_store_si256((__m256i*)(result.p + i + 4), _mm256_permute2x128_si256(lo, hi, 0x31));
    }
    for (; i < count; ++i) result[i] = x[i] + u64(P0) * M::mul(M::sub(y[i], x[i]), inverse);
    return result;
}
}
