#pragma once
#include <toy/convolution.h>
#include <toy/ntt.h>
#include <toy/subset_convolution.h>

namespace toy {

// Truncated modulo each x_i^dimensions[i], first dimension varies fastest.
// Dimensions are >=2, at most 32; the padded NTT length must divide P-1.
template<u32 P = 998244353>
[[gnu::noinline]] Buffer<u32> multivariate_colored(span<const u32> dimensions, Buffer<u32> a, Buffer<u32> b) {
    using M = Mod<P>;
    usize n = a.n, k = dimensions.size();
    if (!k) { a[0] = M::mul(a[0], b[0]); return a; }
    if (k == 1) { a = convolution<P>(std::move(a), std::move(b)); a.n = n; return a; }
    // chi(i)=sum_j floor(i/(d0*...*dj)) mod k. Increment it by the number
    // of carried digits, avoiding division for every coefficient and axis.
    Buffer<u8> color(n);
    array<u32, 32> digit{};
    color[0] = 0;
    for (usize i = 1; i < n; ++i) {
        usize j = 0;
        while (++digit[j] == dimensions[j]) digit[j++] = 0;
        usize c = color[i - 1] + j;
        color[i] = c >= k ? c - k : c;
    }
    if (n <= 32) {
        Buffer<u32> c(n); fill(c.p, c.p + n, 0u);
        for (usize i = 0; i < n; ++i) for (usize j = 0; j < n - i; ++j)
            if ((color[i] + color[j]) % k == color[i + j]) c[i + j] = M::add(c[i + j], M::mul(a[i], b[j]));
        return c;
    }
    usize size = bit_ceil(2 * n - 1);
    Buffer<u32> f(k * size), g(k * size);
    fill(f.p, f.p + f.n, 0u); fill(g.p, g.p + g.n, 0u);
    for (usize i = 0; i < n; ++i) {
        f[color[i] * size + i] = a[i];
        g[color[i] * size + i] = M::mont(b[i], M::r2);
    }
    NTT<P> ntt(size);
    for (usize j = 0; j < k; ++j) {
        ntt.forward(span(f.p + j * size, size));
        ntt.forward(span(g.p + j * size, size));
    }
    for (usize i = 0; i < size; i += 8) {
        // Keep all colors of eight frequencies in L1. Reducing every eight
        // products saves most Montgomery reductions, without u64 overflow.
        alignas(32) __m256i x[32], y[64];
        for (usize j = 0; j < k; ++j) {
            x[j] = _mm256_loadu_si256((const __m256i*)(f.p + j * size + i));
            y[j] = y[j + k] = _mm256_loadu_si256((const __m256i*)(g.p + j * size + i));
        }
        for (usize c = 0; c < k; ++c) {
            auto result = _mm256_setzero_si256();
            for (usize first = 0; first < k; first += 8) {
                auto even = _mm256_setzero_si256(), odd = even;
                for (usize j = first; j < min(first + 8, k); ++j) {
                    auto v = y[k + c - j];
                    even = _mm256_add_epi64(even, _mm256_mul_epu32(x[j], v));
                    odd = _mm256_add_epi64(odd, _mm256_mul_epu32(_mm256_srli_epi64(x[j], 32), _mm256_srli_epi64(v, 32)));
                }
                result = M::add(result, M::mont_sum8(even, odd));
            }
            _mm256_storeu_si256((__m256i*)(f.p + c * size + i), result);
        }
    }
    for (usize j = 0; j < k; ++j) ntt.inverse(span(f.p + j * size, size));
    // A carry changes chi by 1..k-1, so only the matching color survives.
    for (usize i = 0; i < n; ++i) a[i] = f[color[i] * size + i];
    return a;
}
// Keep the binary shortcut separate from the general NTT implementation.
template<u32 P = 998244353>
Buffer<u32> multivariate_convolution(span<const u32> dimensions, Buffer<u32> a, Buffer<u32> b) {
    if (all_of(dimensions.begin(), dimensions.end(), [](u32 d) { return d == 2; }))
        return subset_convolution<P>(std::move(a), std::move(b));
    return multivariate_colored<P>(dimensions, std::move(a), std::move(b));
}

}
