#pragma once
#include <toy/prime.h>
#include <toy/mod.h>

namespace toy {
enum class Divisor { Gcd, Lcm };

// Index zero is unused. Equal lengths, ordinary residues, both arrays consumed.
template<Divisor Kind, u32 P = 998244353>
Buffer<u32> divisor_convolution(Buffer<u32> a, Buffer<u32> b) {
    using M = Mod<P>;
    if (a.n <= 1) return a;
    usize n = a.n - 1;
    auto ps = primes(n);
    auto factor = _mm256_set1_epi32(M::r2);
    usize i = 1;
    for (; i + 8 <= b.n; i += 8)
        _mm256_storeu_si256((__m256i*)(b.p + i), M::mont(_mm256_loadu_si256((const __m256i*)(b.p + i)), factor));
    for (; i <= n; ++i) b[i] = M::mont(b[i], M::r2);
    for (u32 p : span(ps.p, ps.n)) {
        if constexpr (Kind == Divisor::Gcd) {
            // Descending propagates every power of p to its smaller multiples.
            for (usize i = n / p, j = i * p; i; --i, j -= p)
                a[i] = M::add(a[i], a[j]), b[i] = M::add(b[i], b[j]);
        } else {
            for (usize i = 1, j = p; j <= n; ++i, j += p)
                a[j] = M::add(a[j], a[i]), b[j] = M::add(b[j], b[i]);
        }
    }
    for (i = 1; i + 8 <= a.n; i += 8)
        _mm256_storeu_si256((__m256i*)(a.p + i), M::mont(
            _mm256_loadu_si256((const __m256i*)(a.p + i)), _mm256_loadu_si256((const __m256i*)(b.p + i))));
    for (; i <= n; ++i) a[i] = M::mont(a[i], b[i]);
    for (u32 p : span(ps.p, ps.n)) {
        // Reverse each prime's traversal to apply the Mobius inverse.
        if constexpr (Kind == Divisor::Gcd) {
            for (usize i = 1, j = p; j <= n; ++i, j += p) a[i] = M::sub(a[i], a[j]);
        } else {
            for (usize i = n / p, j = i * p; i; --i, j -= p) a[j] = M::sub(a[j], a[i]);
        }
    }
    return a;
}
}
