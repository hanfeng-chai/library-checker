#pragma once
#include <toy/polynomial_gcd.h>

namespace toy {
// Repeated arithmetic modulo one fixed monic polynomial. Cached spectra are
// mutated only by residue normalization; the modulus must have positive degree.
template<u32 P = 998244353>
struct PolynomialMod {
    using M = Mod<P>;
    using E = PolynomialEuclid<P>;
    Buffer<u32> modulus, spectrum, inverse, work, temp;
    usize degree, size;
    explicit PolynomialMod(span<const u32> f) : modulus(E::copy(f)), degree(f.size() - 1),
        size(max<usize>(64, bit_ceil(2 * degree - 1))) {
        fps_scale<P>(modulus, M::pow(f.back(), P - 2));
        if (degree <= 32) return;
        Buffer<u32> reversed(degree + 1); reverse_copy(modulus.p, modulus.p + modulus.n, reversed.p);
        inverse = fps_inv<P>(reversed, degree - 1); inverse.resize(size);
        spectrum = E::copy(modulus); spectrum.resize(size);
        work = Buffer<u32>(size); temp = Buffer<u32>(size);
        transform(inverse); transform(spectrum);
    }
    void transform(Buffer<u32>& a) { convolution_detail::info<P>.forward((__m256i*)a.p, size / 8); }
    void product(Buffer<u32>& a, Buffer<u32>& b) {
        const auto& ntt = convolution_detail::info<P>;
        ntt.products((__m256i*)a.p, (__m256i*)b.p, size / 8); ntt.inverse((__m256i*)a.p, size / 8);
    }
    // Degree at most 2*degree-2, as produced by a product of reduced values.
    Buffer<u32> reduce(Buffer<u32> a) {
        E::trim(a);
        if (a.n <= degree) return a;
        if (degree <= 32) return polynomial_divmod<P>(std::move(a), modulus).second;
        usize count = a.n - degree;
        reverse_copy(a.p + degree, a.p + a.n, work.p); fill(work.p + count, work.p + size, 0u);
        transform(work); product(work, inverse);
        reverse_copy(work.p, work.p + count, temp.p); fill(temp.p + count, temp.p + size, 0u);
        transform(temp); product(temp, spectrum);
        for (usize i = 0; i < degree; ++i) a[i] = M::sub(a[i], temp[i]);
        a.n = degree; E::trim(a); return a;
    }
    Buffer<u32> square(Buffer<u32> a) {
        if (a.n <= 16 || degree <= 32) {
            auto b = E::copy(a); return reduce(convolution<P>(std::move(a), std::move(b)));
        }
        usize count = 2 * a.n - 1;
        a.resize(size); transform(a); memcpy(temp.p, a.p, size * 4); product(a, temp);
        a.n = count; return reduce(std::move(a));
    }
    // (x+c)^exponent. Multiplication by x+c is linear-time; only squaring
    // needs NTT. This is useful for Frobenius tests and randomized splitting.
    Buffer<u32> linear_power(u32 c, u32 exponent) {
        Buffer<u32> a(1); a[0] = 1;
        for (int bit = int(bit_width(exponent)); bit--;) {
            a = square(std::move(a));
            if ((exponent >> bit) & 1) {
                a.resize(a.n + 1);
                for (usize i = a.n; --i;) a[i] = M::add(a[i - 1], M::mul(c, a[i]));
                a[0] = M::mul(c, a[0]);
                if (a.n > degree) {
                    u32 leading = a[degree];
                    for (usize i = 0; i < degree; ++i) a[i] = M::sub(a[i], M::mul(leading, modulus[i]));
                    a.n = degree;
                }
                E::trim(a);
            }
        }
        return a;
    }
};
}
