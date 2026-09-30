#pragma once
#include <toy/bitwise_convolution.h>

namespace toy {
// c[S] = sum_{T subset S} a[T]*b[S\T]. Equal power-of-two lengths <=2^31.
template<bool Divide, u32 P>
Buffer<u32> subset_binary(Buffer<u32> a, Buffer<u32> b) {
    using M = Mod<P>;
    usize n = a.n; if (!n) return a;
    u32 inv = Divide ? M::pow(b[0], P - 2) : 1;
    u32 constant = Divide ? M::mul(a[0], inv) : 0;
    if constexpr (Divide) if (n <= 8) {
        for (usize s = 0; s < n; ++s) {
            u32 value = a[s];
            for (usize t = s; t; t = (t - 1) & s) value = M::sub(value, M::mul(b[t], a[s ^ t]));
            a[s] = M::mul(value, inv);
        }
        return a;
    }
    if (n <= 8) {
        for (usize s = n; s--;) {
            u32 sum = 0;
            for (usize t = s;; t = (t - 1) & s) {
                sum = M::add(sum, M::mul(a[t], b[s ^ t]));
                if (!t) break;
            }
            a[s] = sum;
        }
        return a;
    }
    int bits = countr_zero(n); usize half = n / 2, pitch = half + 16;
    auto storage = [](usize count) {
        if (count * 4 < (1 << 20)) return Buffer<u32>(count);
        usize bytes = (count * 4 + (1 << 21) - 1) & -usize(1 << 21);
        Buffer<u32> a; a.n = a.capacity = count;
        a.p = (u32*)aligned_alloc(1 << 21, bytes); madvise(a.p, bytes, MADV_HUGEPAGE); return a;
    };
    Buffer<u32> f = storage(bits * pitch), g = storage(bits * pitch);
    fill(f.p, f.p + f.n, 0u); fill(g.p, g.p + g.n, 0u);
    u32 normalization = M::pow((P + 1) / 2, bits - 1);
    u32 scale = Divide ? M::r2 : M::mul(M::r2, normalization);
    // For disjoint pairs, |A|+|B|=|A xor B|. Rank parity determines bit 0,
    // so XOR transforms need only mask>>1. Empty factors are added separately.
    for (usize i = 1; i < n; ++i) {
        usize offset = (popcount(i) - 1) * pitch + (i >> 1);
        f[offset] = a[i]; g[offset] = M::mont(b[i], scale);
    }
    for (int r = 0; r < bits; ++r) {
        bitwise_detail::transform<Bitwise::Xor, false, P>(f.p + r * pitch, half);
        bitwise_detail::transform<Bitwise::Xor, false, P>(g.p + r * pitch, half);
    }
    auto q0 = _mm256_set1_epi32(constant), inverse = _mm256_set1_epi32(M::mont(inv, M::r2));
    for (usize i = 0; i < half; i += 8) {
        // Ascending ranks solve a quotient; descending ranks multiply in place.
        for (int k = Divide ? 0 : 1; k < bits; ++k) {
            int r = Divide ? k : bits - k;
            auto sum = _mm256_setzero_si256();
            if constexpr (Divide) sum = M::mont(_mm256_loadu_si256((const __m256i*)(g.p + r * pitch + i)), q0);
            for (int first = 0; first < r; first += 8) {
                auto even = _mm256_setzero_si256(), odd = even;
                for (int j = first; j < min(first + 8, r); ++j) {
                    auto x = _mm256_loadu_si256((const __m256i*)(f.p + j * pitch + i));
                    auto y = _mm256_loadu_si256((const __m256i*)(g.p + (r - 1 - j) * pitch + i));
                    even = _mm256_add_epi64(even, _mm256_mul_epu32(x, y));
                    odd = _mm256_add_epi64(odd, _mm256_mul_epu32(_mm256_srli_epi64(x, 32), _mm256_srli_epi64(y, 32)));
                }
                sum = M::add(sum, M::mont_sum8(even, odd));
            }
            if constexpr (Divide) {
                sum = M::sub(_mm256_loadu_si256((const __m256i*)(f.p + r * pitch + i)), sum);
                if (inv != 1) sum = M::mont(sum, inverse);
            }
            _mm256_storeu_si256((__m256i*)(f.p + r * pitch + i), sum);
        }
        if constexpr (!Divide) _mm256_storeu_si256((__m256i*)(f.p + i), _mm256_setzero_si256());
    }
    for (int r = Divide ? 0 : 1; r < bits; ++r) bitwise_detail::transform<Bitwise::Xor, true, P>(f.p + r * pitch, half);
    u32 left = a[0], right = b[0];
    auto l = _mm256_set1_epi32(M::mont(left, M::r2)), r = _mm256_set1_epi32(M::mont(right, M::r2));
    auto factor = _mm256_set1_epi32(M::mont(normalization, M::r2));
    for (usize i = 0; i < n; i += 8) {
        alignas(32) u32 product[8];
        for (usize j = 0; j < 8; ++j) product[j] = i + j ? f[(popcount(i + j) - 1) * pitch + ((i + j) >> 1)] : 0;
        auto value = _mm256_load_si256((const __m256i*)product);
        if constexpr (Divide) value = M::mont(value, factor);
        else {
            auto x = _mm256_loadu_si256((const __m256i*)(a.p + i));
            auto y = _mm256_loadu_si256((const __m256i*)(b.p + i));
            value = M::add(value, M::add(M::mont(x, r), M::mont(y, l)));
        }
        _mm256_storeu_si256((__m256i*)(a.p + i), value);
    }
    a[0] = Divide ? constant : M::mul(left, right);
    return a;
}

template<u32 P = 998244353>
Buffer<u32> subset_convolution(Buffer<u32> a, Buffer<u32> b) {
    return subset_binary<false, P>(std::move(a), std::move(b));
}
template<u32 P = 998244353>
Buffer<u32> subset_division(span<const u32> a, span<const u32> b) {
    Buffer<u32> x(a.size()), y(b.size());
    memcpy(x.p, a.data(), a.size() * 4); memcpy(y.p, b.data(), b.size() * 4);
    return subset_binary<true, P>(std::move(x), std::move(y));
}

template<u32 P = 998244353>
Buffer<u32> subset_convolution(span<const u32> a, span<const u32> b) {
    Buffer<u32> x(a.size()), y(b.size());
    memcpy(x.p, a.data(), a.size() * 4); memcpy(y.p, b.data(), b.size() * 4);
    return subset_convolution<P>(std::move(x), std::move(y));
}
}
