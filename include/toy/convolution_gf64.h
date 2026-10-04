#pragma once
#include <toy/buffer.h>
#include <toy/gf64.h>

namespace toy {
namespace gf64_detail {
struct FFT {
    Buffer<u64> points, scratch;
    explicit FFT(usize size) : points(size), scratch(2 * size) {
        for (int k = 2; (usize(1) << k) <= size; ++k) {
            u64 *p = points.p + (usize(1) << (k - 1));
            p[0] = 0;
            for (int j = 0; j < k - 1; ++j)
                for (usize i = 0; i < (usize(1) << j); ++i)
                    p[(usize(1) << j) + i] = p[i] ^ chain[64 - k + j];
        }
    }
    template <bool Inverse>
    static void shift(u64 *f, usize n) {
        // Convert between powers of x and the two parity parts in x^2+x.
        for (usize len = Inverse ? 1 : n / 4; len && 4 * len <= n;
             len = Inverse ? len * 2 : len / 2)
            for (usize s = 0; s < n; s += 4 * len)
                for (usize i = 0; i < len; ++i) {
                    u64 b = f[s + len + i], c = f[s + 2 * len + i], d = f[s + 3 * len + i];
                    f[s + len + i] = Inverse ? b ^ c : b ^ c ^ d;
                    f[s + 2 * len + i] = c ^ d;
                }
    }
    template <bool Inverse>
    [[gnu::target("pclmul")]] void transform(u64 *f, usize n, u64 *work) const {
        if (n == 1) return;
        if (n == 2) {
            f[1] ^= f[0];
            return;
        }
        usize half = n / 2;
        const u64 *p = points.p + half;
        u64 *even = work;
        u64 *odd = work + half;
        if constexpr (!Inverse) {
            shift<false>(f, n);
            for (usize i = 0; i < half; ++i) even[i] = f[2 * i], odd[i] = f[2 * i + 1];
        } else {
            for (usize i = 0; i < half; ++i) {
                odd[i] = f[i] ^ f[i + half];
                even[i] = f[i] ^ gf64_mul(p[i], odd[i]);
            }
        }
        transform<Inverse>(even, half, work + n);
        transform<Inverse>(odd, half, work + n);
        if constexpr (!Inverse) {
            for (usize i = 0; i < half; ++i) {
                f[i] = even[i] ^ gf64_mul(p[i], odd[i]);
                f[i + half] = f[i] ^ odd[i];
            }
        } else {
            for (usize i = 0; i < half; ++i) f[2 * i] = even[i], f[2 * i + 1] = odd[i];
            shift<true>(f, n);
        }
    }
};
} // namespace gf64_detail

// Ordinary polynomial-basis field elements. Consumes both buffers.
[[gnu::target("pclmul")]] inline Buffer<u64> convolution_gf64(Buffer<u64> a, Buffer<u64> b) {
    if (!a.n || !b.n) return {};
    if (a.n < b.n) std::swap(a, b);
    usize na = a.n, nb = b.n, count = na + nb - 1, size = std::bit_ceil(count);
    if (nb <= 32) {
        a.resize(count);
        for (usize k = count; k--;) {
            u64 sum = 0;
            for (usize j = k < na ? 0 : k - na + 1; j < std::min(nb, k + 1); ++j)
                sum ^= gf64_mul(a[k - j], b[j]);
            a[k] = sum;
        }
        return a;
    }
    if (count == size / 2 + 1) {
        // Remove a one-coefficient tail to halve the transform, then restore it.
        Buffer<u64> tail(nb);
        u64 last = a[--a.n];
        for (usize i = 0; i < nb; ++i) tail[i] = gf64_mul(last, b[i]);
        auto c = convolution_gf64(std::move(a), std::move(b));
        c.resize(count);
        for (usize i = 0; i < nb; ++i) c[na - 1 + i] ^= tail[i];
        return c;
    }
    a.resize(size);
    b.resize(size);
    gf64_detail::FFT fft(size);
    fft.transform<false>(a.p, size, fft.scratch.p);
    fft.transform<false>(b.p, size, fft.scratch.p);
    for (usize i = 0; i < size; ++i) a[i] = gf64_mul(a[i], b[i]);
    fft.transform<true>(a.p, size, fft.scratch.p);
    a.n = count;
    return a;
}
} // namespace toy
