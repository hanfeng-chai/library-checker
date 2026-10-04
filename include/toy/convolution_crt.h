#pragma once
#include <toy/convolution.h>
#include <toy/mod.h>

namespace toy {
namespace crt_detail {
template <u32 Q, class T>
Buffer<u32> under_prime(std::span<const T> a, std::span<const T> b) {
    usize size = std::bit_ceil(a.size() + b.size() - 1);
    Buffer<u32> x(a.size(), size), y(b.size(), size);
    for (usize i = 0; i < a.size(); ++i) x[i] = a[i] % Q;
    for (usize i = 0; i < b.size(); ++i) y[i] = b[i] % Q;
    return convolution<Q>(std::move(x), std::move(y));
}
} // namespace crt_detail

// Exact integer reconstruction: min(n,m)*(P-1)^2 must be smaller than
// 998244353*1004535809*469762049. Coefficients are ordinary residues modulo P.
template <u32 P>
Buffer<u32> convolution_crt(std::span<const u32> a, std::span<const u32> b) {
    static_assert(P > 1 && P < (1u << 30));
    if (a.empty() || b.empty()) return {};
    usize count = a.size() + b.size() - 1;
    if (std::min(a.size(), b.size()) <= 16) {
        if (a.size() < b.size()) std::swap(a, b);
        Buffer<u32> c(count);
        for (usize k = 0; k < count; ++k) {
            u64 sum = 0;
            for (usize j = k < a.size() ? 0 : k - a.size() + 1; j < std::min(b.size(), k + 1); ++j)
                sum += u64(a[k - j]) * b[j];
            c[k] = sum % P;
        }
        return c;
    }
    constexpr u32 p0 = 998244353, p1 = 1004535809, p2 = 469762049;
    constexpr u32 inv01 = Mod<p1>::pow(p0, p1 - 2);
    constexpr u32 inv012 = Mod<p2>::pow(u64(p0) * p1 % p2, p2 - 2);
    auto c0 = crt_detail::under_prime<p0>(a, b);
    auto c1 = crt_detail::under_prime<p1>(a, b);
    auto c2 = crt_detail::under_prime<p2>(a, b);
    for (usize i = 0; i < count; ++i) {
        u32 x = Mod<p1>::mul(Mod<p1>::sub(c1[i], c0[i]), inv01);
        u32 y = Mod<p2>::mul(Mod<p2>::sub(c2[i], (c0[i] + u64(p0) * x) % p2), inv012);
        c0[i] = (c0[i] + u64(p0 % P) * x + u64(u64(p0) * p1 % P) * y) % P;
    }
    return c0;
}

// Five primes reconstruct the full integer coefficient before reducing modulo
// 2^64. Sufficient bound: min(n,m) <= 2^20, transform length <= 2^24.
inline Buffer<u64> convolution_u64_basic(std::span<const u64> a, std::span<const u64> b) {
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
    auto c0 = crt_detail::under_prime<p0>(a, b), c1 = crt_detail::under_prime<p1>(a, b);
    auto c2 = crt_detail::under_prime<p2>(a, b), c3 = crt_detail::under_prime<p3>(a, b);
    auto c4 = crt_detail::under_prime<p4>(a, b);
    for (usize i = 0; i < count; ++i) {
        u32 t0 = c0[i], t1 = Mod<p1>::mul(Mod<p1>::sub(c1[i], t0), inv1);
        u64 x = t0 + u64(p0) * t1;
        u32 t2 = Mod<p2>::mul(Mod<p2>::sub(c2[i], x % p2), inv2);
        u32 t3 = Mod<p3>::mul(Mod<p3>::sub(c3[i], (x + p01 % p3 * t2) % p3), inv3);
        u32 t4 = Mod<p4>::mul(
            Mod<p4>::sub(c4[i], (x + p01 % p4 * t2 + p01 % p4 * p2 % p4 * t3) % p4), inv4);
        c[i] = x + p01 * t2 + p01 * p2 * t3 + p01 * p2 * p3 * t4;
    }
    return c;
}

// Repeated cyclic products with a fixed right operand, 1 < runtime modulus <2^30.
// The length is a power of two in [64,2^24]. Three primes reconstruct exactly.
struct CyclicCRT {
    static constexpr u32 p0 = 998244353, p1 = 1004535809, p2 = 469762049;
    u32 modulus;
    std::array<Buffer<u32>, 3> spectrum;
    template <u32 Q>
    static Buffer<u32> prepare(std::span<const u32> input) {
        Buffer<u32> f(input.size());
        for (usize i = 0; i < f.n; ++i) f[i] = input[i] % Q;
        convolution_detail::info<Q>.forward((convolution_detail::Vec *)f.p, f.n / 8);
        return f;
    }
    CyclicCRT(std::span<const u32> b, u32 p)
        : modulus(p), spectrum{prepare<p0>(b), prepare<p1>(b), prepare<p2>(b)} {}
    template <u32 Q>
    static Buffer<u32> run(std::span<const u32> a, Buffer<u32> &b) {
        auto f = prepare<Q>(a);
        auto *x = (convolution_detail::Vec *)f.p;
        const auto &ntt = convolution_detail::info<Q>;
        ntt.products(x, (convolution_detail::Vec *)b.p, f.n / 8);
        ntt.inverse(x, f.n / 8);
        return f;
    }
    Buffer<u32> operator()(std::span<const u32> a) {
        auto c0 = run<p0>(a, spectrum[0]), c1 = run<p1>(a, spectrum[1]),
             c2 = run<p2>(a, spectrum[2]);
        constexpr u32 inv01 = Mod<p1>::pow(p0, p1 - 2);
        constexpr u32 inv012 = Mod<p2>::pow(u64(p0) * p1 % p2, p2 - 2);
        Barrett mod(modulus);
        u32 w1 = p0 % modulus, w2 = u64(p0) * p1 % modulus;
        for (usize i = 0; i < c0.n; ++i) {
            u32 x = Mod<p1>::mul(Mod<p1>::sub(c1[i], c0[i]), inv01);
            u32 y = Mod<p2>::mul(Mod<p2>::sub(c2[i], (c0[i] + u64(p0) * x) % p2), inv012);
            u32 v = mod.mul(c0[i], 1) + mod.mul(w1, x) + mod.mul(w2, y);
            v = std::min(v, v - 2 * modulus);
            c0[i] = std::min(v, v - modulus);
        }
        return c0;
    }
};
} // namespace toy
