#pragma once
#include <toy/radix_ntt.h>
#include <toy/fps.h>

namespace toy {
// f(g(x)) mod x^n, g(0)=0. Large inputs require 4*bit_ceil(n) roots.
template<u32 P = 998244353>
Buffer<u32> fps_compose(std::span<const u32> f, std::span<const u32> g, usize n) {
    using M = Mod<P>;
    Buffer<u32> result(n); if (!n) return result;
    std::fill(result.p, result.p + n, 0u);
    f = f.first(std::min(f.size(), n)); g = g.first(std::min(g.size(), n));
    if (f.empty()) return result;
    usize nonzero = 0, degree = 0;
    for (usize i = 1; i < g.size(); ++i) if (g[i]) ++nonzero, degree = i;
    if (nonzero <= 1) {
        result[0] = f[0]; u32 power = 1;
        if (nonzero) for (usize i = 1; i < f.size() && i * degree < n; ++i) {
            power = M::mul(power, g[degree]); result[i * degree] = M::mul(f[i], power);
        }
        return result;
    }
    if (n <= 16) {
        Buffer<u32> next(n);
        for (usize i = f.size(); i--;) {
            std::fill(next.p, next.p + n, 0u);
            for (usize j = 0; j < n; ++j) for (usize k = 1; k < g.size() && j + k < n; ++k)
                next[j + k] = M::add(next[j + k], M::mul(result[j], g[k]));
            next[0] = f[i]; std::swap(result, next);
        }
        return result;
    }
    usize length = std::bit_ceil(n); constexpr u32 one = (u64(1) << 32) % P;
    RadixNTT<P> ntt(4 * length);
    Buffer<u32> outer(length), q(length);
    std::fill(outer.p, outer.p + length, 0u); std::fill(q.p, q.p + length, 0u);
    for (usize i = 0; i < f.size(); ++i) outer[i] = M::mont(f[i], M::r2);
    for (usize i = 0; i < g.size(); ++i) q[i] = M::sub(0, M::mont(g[i], M::r2));
    // Monic denominator y^m+Q(x,y). Pair x and -x, halve x precision,
    // double y degree; m*width remains length. The monic term stays implicit.
    auto solve = [&](auto&& self, Buffer<u32> q, usize m, usize width) -> Buffer<u32> {
        if (width == 1) {
            Buffer<u32> value(length); memcpy(value.p, outer.p, length * 4); return value;
        }
        Buffer<u32> spectrum(4 * length), next(2 * length);
        std::fill(spectrum.p, spectrum.p + spectrum.n, 0u);
        for (usize i = 0; i < m; ++i) memcpy(spectrum.p + 2 * i * width, q.p + i * width, width * 4);
        spectrum[2 * length] = one; q = Buffer<u32>{}; ntt.forward(spectrum);
        auto order = _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7);
        for (usize i = 0; i < 2 * length; i += 8) {
            auto a = _mm256_permutevar8x32_epi32(_mm256_load_si256((const __m256i*)(spectrum.p + 2 * i)), order);
            auto b = _mm256_permutevar8x32_epi32(_mm256_load_si256((const __m256i*)(spectrum.p + 2 * i + 8)), order);
            _mm256_store_si256((__m256i*)(next.p + i), M::mont(
                _mm256_permute2x128_si256(a, b, 0x20), _mm256_permute2x128_si256(a, b, 0x31)));
        }
        ntt.inverse(next); next[0] = M::sub(next[0], one); // Undo cyclic wrap of the monic term.
        for (usize i = 1; i < 2 * m; ++i) memcpy(next.p + i * (width / 2), next.p + i * width, width * 2);
        next.n = length;
        auto value = self(self, std::move(next), 2 * m, width / 2);
        Buffer<u32> work(2 * length); std::fill(work.p, work.p + work.n, 0u);
        for (usize i = 0; i < 2 * m; ++i) memcpy(work.p + i * width, value.p + i * (width / 2), width * 2);
        value = Buffer<u32>{}; ntt.forward(work);
        // Transpose the odd-coefficient extraction: duplicate each weight,
        // swap paired denominator frequencies, and retain the high y half.
        for (usize i = 0; i < 2 * length; i += 8) {
            auto v = _mm256_load_si256((const __m256i*)(work.p + i));
            auto lo = _mm256_unpacklo_epi32(v, v), hi = _mm256_unpackhi_epi32(v, v);
            auto a = _mm256_shuffle_epi32(_mm256_load_si256((const __m256i*)(spectrum.p + 2 * i)), _MM_SHUFFLE(2, 3, 0, 1));
            auto b = _mm256_shuffle_epi32(_mm256_load_si256((const __m256i*)(spectrum.p + 2 * i + 8)), _MM_SHUFFLE(2, 3, 0, 1));
            _mm256_store_si256((__m256i*)(spectrum.p + 2 * i), M::mont(a, _mm256_permute2x128_si256(lo, hi, 0x20)));
            _mm256_store_si256((__m256i*)(spectrum.p + 2 * i + 8), M::mont(b, _mm256_permute2x128_si256(lo, hi, 0x31)));
        }
        ntt.inverse(spectrum); Buffer<u32> answer(length);
        for (usize i = 0; i < m; ++i) memcpy(answer.p + i * width, spectrum.p + 2 * (i + m) * width, width * 4);
        return answer;
    };
    auto value = solve(solve, std::move(q), 1, length);
    for (usize i = 0; i < n; ++i) result[i] = M::mont(value[i], 1);
    return result;
}

// For k=0..weights.size()-1, sum_j weights[j]*[x^j]f(x)^k; f(0)=0.
template<u32 P = 998244353>
Buffer<u32> fps_power_projection(std::span<const u32> f, std::span<const u32> weights) {
    using M = Mod<P>;
    usize n = weights.size(); Buffer<u32> result(n); if (!n) return result;
    f = f.first(std::min(f.size(), n));
    if (n <= 16) {
        Buffer<u32> a(n), b(n); std::fill(a.p, a.p + n, 0u); a[0] = 1;
        for (usize k = 0; k < n; ++k) {
            u64 value = 0; for (usize i = 0; i < n; ++i) value += u64(weights[i]) * a[i];
            result[k] = value % P; std::fill(b.p, b.p + n, 0u);
            for (usize i = 0; i < n; ++i) for (usize j = 1; j < f.size() && i + j < n; ++j)
                b[i + j] = M::add(b[i + j], M::mul(a[i], f[j]));
            std::swap(a, b);
        }
        return result;
    }
    usize length = std::bit_ceil(n); constexpr u32 one = (u64(1) << 32) % P;
    RadixNTT<P> ntt(4 * length);
    Buffer<u32> q(length), a(length), qs(4 * length), as(4 * length), next(2 * length);
    std::fill(q.p, q.p + length, 0u); std::fill(a.p, a.p + length, 0u);
    for (usize i = 0; i < f.size(); ++i) q[i] = M::sub(0, M::mont(f[i], M::r2));
    for (usize i = 0; i < n; ++i) a[length - 1 - i] = M::mont(weights[i], M::r2);
    for (usize width = length, m = 1; width > 1; width /= 2, m *= 2) {
        std::fill(qs.p, qs.p + qs.n, 0u); std::fill(as.p, as.p + as.n, 0u);
        for (usize i = 0; i < m; ++i) {
            memcpy(qs.p + 2 * i * width, q.p + i * width, width * 4);
            memcpy(as.p + 2 * i * width, a.p + i * width, width * 4);
        }
        qs[2 * length] = one; ntt.forward(qs); ntt.forward(as);
        auto order = _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7);
        for (usize i = 0; i < 2 * length; i += 8) {
            auto x = _mm256_permutevar8x32_epi32(_mm256_load_si256((const __m256i*)(qs.p + 2 * i)), order);
            auto y = _mm256_permutevar8x32_epi32(_mm256_load_si256((const __m256i*)(qs.p + 2 * i + 8)), order);
            auto qe = _mm256_permute2x128_si256(x, y, 0x20), qo = _mm256_permute2x128_si256(x, y, 0x31);
            _mm256_store_si256((__m256i*)(next.p + i), M::mont(qe, qo));
            auto u = _mm256_permutevar8x32_epi32(_mm256_load_si256((const __m256i*)(as.p + 2 * i)), order);
            auto v = _mm256_permutevar8x32_epi32(_mm256_load_si256((const __m256i*)(as.p + 2 * i + 8)), order);
            auto pe = _mm256_permute2x128_si256(u, v, 0x20), po = _mm256_permute2x128_si256(u, v, 0x31);
            // Odd coefficients at omega^2 equal (R(omega)-R(-omega))/(2*omega).
            // Pairing here halves the following inverse transform.
            auto value = M::mont(M::sub(M::mont(pe, qo), M::mont(po, qe)),
                _mm256_load_si256((const __m256i*)(ntt.inverse_root.p + i)));
            auto odd = _mm256_and_si256(value, _mm256_set1_epi32(1));
            value = _mm256_srli_epi32(_mm256_add_epi32(value, _mm256_mullo_epi32(odd, _mm256_set1_epi32(P))), 1);
            _mm256_store_si256((__m256i*)(as.p + i), value);
        }
        ntt.inverse(next); next[0] = M::sub(next[0], one); ntt.inverse(std::span<u32>(as.p, 2 * length));
        for (usize i = 0; i < 2 * m; ++i) for (usize j = 0; j < width / 2; ++j) {
            q[i * (width / 2) + j] = next[i * width + j];
            a[i * (width / 2) + j] = as[i * width + j];
        }
    }
    for (usize i = 0; i < n; ++i) result[i] = M::mont(a[length - 1 - i], 1);
    return result;
}

// g with f(g(x))=x mod x^n; f(0)=0, f'(0)!=0 and n<P.
template<u32 P = 998244353>
Buffer<u32> fps_revert(std::span<const u32> f, usize n) {
    using M = Mod<P>;
    Buffer<u32> result(n); if (!n) return result;
    std::fill(result.p, result.p + n, 0u); if (n == 1) return result;
    usize count = std::min(f.size(), n);
    while (count > 2 && !f[count - 1]) --count;
    u32 inv = M::pow(f[1], P - 2);
    if (count == 2) { result[1] = inv; return result; }
    Buffer<u32> weights(n); std::fill(weights.p, weights.p + n, 0u); weights[n - 1] = 1;
    auto projection = fps_power_projection<P>(f, weights);
    auto inverse = inverse_numbers<P, true>(n - 1); Buffer<u32> b(n - 1);
    // Lagrange inversion with fixed M=n-1:
    // [t^(M-i)](t/g)^M = M/i * [x^M]f(x)^i, for 1<=i<=M.
    for (usize i = 1; i < n; ++i) b[n - 1 - i] = M::mont(projection[i], inverse[i]);
    fps_scale<P>(b, M::mul(n - 1, M::pow(inv, n - 1)));
    auto h = fps_pow<P>(b, n - 1, P - M::pow(n - 1, P - 2));
    fps_scale<P>(h, inv); memcpy(result.p + 1, h.p, (n - 1) * 4); return result;
}
}
