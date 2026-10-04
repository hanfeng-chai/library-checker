#pragma once
#include <toy/convolution_fast.h>
#ifndef TOY_CRT_FUSED_NTT
#define TOY_CRT_FUSED_NTT 1
#endif

namespace toy {
namespace u64_detail {
template <u32 Q, class T>
[[gnu::noinline, gnu::hot]] Buffer<u32> under_prime(std::span<const T> a, std::span<const T> b) {
    usize size = std::bit_ceil(a.size() + b.size() - 1);
    auto x = ntt_storage<u32>(a.size(), size + 16), y = ntt_storage<u32>(b.size(), size + 16);
    auto fill = [&](u32 *out, std::span<const T> in) {
        usize i = 0;
        if constexpr (sizeof(T) == 8 && Q > (1u << 29)) {
            using N = ProductNTT<Q>;
            using V = __m256i;
            typename N::Fixed high(N::fixed((u64(1) << 32) % Q));
            for (; i + 8 <= in.size(); i += 8) {
                V a = _mm256_loadu_si256((const V *)(in.data() + i)),
                  b = _mm256_loadu_si256((const V *)(in.data() + i + 4));
                V lo = _mm256_castps_si256(
                    _mm256_shuffle_ps(_mm256_castsi256_ps(a), _mm256_castsi256_ps(b), 0x88));
                V hi = _mm256_castps_si256(
                    _mm256_shuffle_ps(_mm256_castsi256_ps(a), _mm256_castsi256_ps(b), 0xdd));
                lo = _mm256_permute4x64_epi64(lo, 0xd8);
                hi = _mm256_permute4x64_epi64(hi, 0xd8);
                // x=lo+2^32*hi. Both terms are reduced below 2Q before addition.
                lo = N::low(_mm256_min_epu32(lo, N::sub(lo, N::splat(4 * Q))));
                _mm256_store_si256((V *)(out + i), N::canon(N::add(lo, high(hi))));
            }
        }
        for (; i < in.size(); ++i) out[i] = in[i] % Q;
    };
    fill(x.p, a);
    fill(y.p, b);
#if TOY_CRT_FUSED_NTT
    return convolution_fast<Q>(std::move(x), std::move(y));
#else
    return convolution<Q>(std::move(x), std::move(y));
#endif
}
} // namespace u64_detail

// Three to five primes reconstruct the full coefficient before taking low 64
// bits. Sufficient bound: min(n,m) <= 2^20, padded length <= 2^24.
inline Buffer<u64> convolution_u64(std::span<const u64> a, std::span<const u64> b) {
    if (a.empty() || b.empty()) return {};
    usize count = a.size() + b.size() - 1;
    Buffer<u64> c(count);
    if (std::min(a.size(), b.size()) <= 16) {
        if (a.size() < b.size()) std::swap(a, b);
        for (usize k = 0; k < count; ++k) {
            u64 sum = 0;
            for (usize j = k < a.size() ? 0 : k - a.size() + 1; j < std::min(b.size(), k + 1); ++j)
                sum += a[k - j] * b[j];
            c[k] = sum;
        }
        return c;
    }
    constexpr u32 p0 = 754974721, p1 = 880803841, p2 = 897581057, p3 = 998244353, p4 = 1004535809;
    constexpr u64 p01 = u64(p0) * p1;
    constexpr u32 inv1 = Mod<p1>::pow(p0, p1 - 2);
    constexpr u32 inv2 = Mod<p2>::pow(p01 % p2, p2 - 2);
    constexpr u32 inv3 = Mod<p3>::pow(p01 % p3 * p2 % p3, p3 - 2);
    constexpr u32 inv4 = Mod<p4>::pow(p01 % p4 * p2 % p4 * p3 % p4, p4 - 2);
    u64 ma = *std::max_element(a.begin(), a.end()), mb = *std::max_element(b.begin(), b.end());
    if (!ma || !mb) {
        std::fill(c.p, c.p + count, 0);
        return c;
    }
    u128 p012 = u128(p01) * p2, p0123 = p012 * p3;
    // A coefficient is at most min(n,m)*max(a)*max(b). Divide the modulus
    // product before comparing, so the bound itself cannot overflow u128.
    bool need3 = ma > (p012 - 1) / std::min(a.size(), b.size()) / mb;
    bool need4 = ma > (p0123 - 1) / std::min(a.size(), b.size()) / mb;
    auto c0 = u64_detail::under_prime<p0>(a, b), c1 = u64_detail::under_prime<p1>(a, b);
    auto c2 = u64_detail::under_prime<p2>(a, b);
    Buffer<u32> c3, c4;
    if (need3) c3 = u64_detail::under_prime<p3>(a, b);
    if (need4) c4 = u64_detail::under_prime<p4>(a, b);
    auto reconstruct = [&]<int K> [[gnu::noinline, gnu::hot]] () {
        for (usize i = 0; i < count; ++i) {
            u32 t0 = c0[i], t1 = Mod<p1>::mul(Mod<p1>::sub(c1[i], t0), inv1);
            u64 x = t0 + u64(p0) * t1;
            u32 t2 = Mod<p2>::mul(Mod<p2>::sub(c2[i], x % p2), inv2);
            u64 value = x + p01 * t2;
            if constexpr (K >= 4) {
                u32 t3 = Mod<p3>::mul(Mod<p3>::sub(c3[i], (x + p01 % p3 * t2) % p3), inv3);
                value += p01 * p2 * t3;
                if constexpr (K >= 5) {
                    u32 t4 = Mod<p4>::mul(
                        Mod<p4>::sub(c4[i], (x + p01 % p4 * t2 + p01 % p4 * p2 % p4 * t3) % p4),
                        inv4);
                    value += p01 * p2 * p3 * t4;
                }
            }
            c[i] = value;
        }
    };
    if (need4)
        reconstruct.template operator()<5>();
    else if (need3)
        reconstruct.template operator()<4>();
    else
        reconstruct.template operator()<3>();
    return c;
}

} // namespace toy
