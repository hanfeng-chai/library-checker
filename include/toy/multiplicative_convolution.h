#pragma once
#include <toy/convolution.h>
#include <toy/primitive_root.h>
#include <toy/ntt.h>

namespace toy {
// Equal prime lengths p. c[k] = sum_{i*j mod p = k} a[i]*b[j] modulo Q.
template<u32 Q = 998244353>
Buffer<u32> multiplicative_convolution_prime(std::span<const u32> a, std::span<const u32> b) {
    using M = Mod<Q>;
    u32 p = a.size(), n = p - 1, g = primitive_root(p);
    Barrett mod(p);
    usize size = std::bit_ceil(usize(2) * n - 1);
    Buffer<u32> x(n, size), y(n, size), c(p);
    u64 sa = 0, sb = 0;
    for (u32 i = 0, k = 1; i < n; ++i, k = mod.mul(k, g)) {
        x[i] = a[k]; y[i] = b[k];
        sa += a[k]; sb += b[k];
    }
    c[0] = M::add(M::mul(a[0], M::add(b[0], sb % Q)), M::mul(b[0], sa % Q));
    x = convolution<Q>(std::move(x), std::move(y));
    for (usize i = n; i < x.n; ++i) x[i - n] = M::add(x[i - n], x[i]);
    for (u32 i = 0, k = 1; i < n; ++i, k = mod.mul(k, g)) c[k] = x[i];
    return c;
}

// Equal power-of-two lengths. Split nonzero indices as 2^v * (-1)^s * 5^j.
template<u32 Q = 998244353>
Buffer<u32> multiplicative_convolution_2n(std::span<const u32> a, std::span<const u32> b) {
    using M = Mod<Q>;
    usize n = a.size(), mask = n - 1;
    int log = std::countr_zero(n);
    NTT<Q> ntt(std::max<usize>(1, n / 4));
    Buffer<u32> x(n), y(n), z(n);
    std::fill(z.p, z.p + n, 0u);
    std::array<usize, 64> offset, width, height;
    usize pos = 0;
    for (int v = 0; v <= log; ++v) {
        usize w = usize(1) << std::max(log - v - 2, 0), h = log - v >= 2 ? 2 : 1;
        offset[v] = pos; width[v] = w; height[v] = h;
        for (usize j = 0, k = (usize(1) << v) & mask; j < w; ++j, k = 5 * k & mask) {
            x[pos + j] = a[k]; y[pos + j] = M::mont(b[k], M::r2);
            if (h == 2) {
                x[pos + w + j] = a[n - k]; y[pos + w + j] = M::mont(b[n - k], M::r2);
            }
        }
        for (usize s = 0; s < h; ++s) {
            ntt.forward(std::span(x.p + pos + s * w, w));
            ntt.forward(std::span(y.p + pos + s * w, w));
        }
        if (h == 2) for (usize j = 0; j < w; ++j) {
            u32 a0 = x[pos + j], a1 = x[pos + w + j], b0 = y[pos + j], b1 = y[pos + w + j];
            x[pos + j] = M::add(a0, a1); x[pos + w + j] = M::sub(a0, a1);
            y[pos + j] = M::add(b0, b1); y[pos + w + j] = M::sub(b0, b1);
        }
        pos += h * w;
    }
    for (int i = 0; i <= log; ++i) for (int j = 0; j <= log; ++j) {
        int k = std::min(log, i + j);
        // Bit-reversed frequency prefixes are precisely the smaller cyclic
        // quotient's frequencies, so no extra folding transforms are needed.
        for (usize s = 0; s < height[k]; ++s) {
            const u32* u = x.p + offset[i] + s * width[i];
            const u32* v = y.p + offset[j] + s * width[j];
            u32* c = z.p + offset[k] + s * width[k];
            usize t = 0, w = width[k];
            for (; t + 8 <= w; t += 8) _mm256_storeu_si256((__m256i*)(c + t), M::add(
                _mm256_loadu_si256((const __m256i*)(c + t)), M::mont(
                _mm256_loadu_si256((const __m256i*)(u + t)), _mm256_loadu_si256((const __m256i*)(v + t)))));
            for (; t < w; ++t) c[t] = M::add(c[t], M::mont(u[t], v[t]));
        }
    }
    for (int v = 0; v <= log; ++v) {
        usize pos = offset[v], w = width[v], h = height[v];
        for (usize s = 0; s < h; ++s) ntt.inverse(std::span(z.p + pos + s * w, w), h == 2 ? (Q + 1) / 2 : 1);
        for (usize j = 0, k = (usize(1) << v) & mask; j < w; ++j, k = 5 * k & mask) {
            u32 a0 = z[pos + j];
            if (h == 2) {
                u32 a1 = z[pos + w + j];
                x[k] = M::add(a0, a1); x[n - k] = M::sub(a0, a1);
            } else x[k] = a0;
        }
    }
    return x;
}
}
