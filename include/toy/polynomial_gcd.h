#pragma once
#include <toy/polynomial.h>

namespace toy {
template<u32 P = 998244353>
struct PolynomialEuclid {
    using M = Mod<P>;
    using Poly = Buffer<u32>;
    using Matrix = std::array<Poly, 4>;
    static void trim(Poly& a) { while (a.n && !a[a.n - 1]) --a.n; }
    static Poly copy(std::span<const u32> a) {
        Poly result(a.size()); if (!a.empty()) memcpy(result.p, a.data(), a.size() * 4);
        return result;
    }
    static Matrix identity() {
        Matrix r; r[0] = Poly(1); r[3] = Poly(1); r[0][0] = r[3][0] = 1;
        return r;
    }
    static Poly subtract_product(Poly a, std::span<const u32> b, std::span<const u32> c) {
        if (b.empty() || c.empty()) return a;
        if (b.size() > c.size()) std::swap(b, c);
        if (b.size() <= 16) {
            usize size = b.size() + c.size() - 1; a.resize(std::max(a.n, size));
            for (usize k = 0; k < size; ++k) {
                u64 sum = 0;
                for (usize j = k < c.size() ? 0 : k - c.size() + 1; j < std::min(b.size(), k + 1); ++j)
                    sum += u64(b[j]) * c[k - j];
                a[k] = M::sub(a[k], sum % P);
            }
            trim(a); return a;
        }
        auto product = convolution<P>(copy(b), copy(c)); a.resize(std::max(a.n, product.n));
        for (usize i = 0; i < product.n; ++i) a[i] = M::sub(a[i], product[i]);
        trim(a); return a;
    }
    // Multiply a 2x2 polynomial matrix by a 2xC matrix. Each input transform
    // is shared by both output rows; sum the two products before inversion.
    template<usize C>
    static std::array<Poly, 2 * C> multiply(const Matrix& a, const std::array<Poly, 2 * C>& b) {
        std::array<Poly, 2 * C> result;
        usize an = 0, bn = 0;
        for (const auto& x : a) an = std::max(an, x.n);
        for (const auto& x : b) bn = std::max(bn, x.n);
        if (!an || !bn) return result;
        if (std::min(an, bn) <= 16) {
            for (usize i = 0; i < 2; ++i) for (usize j = 0; j < C; ++j) {
                auto x = convolution<P>(copy(a[2 * i]), copy(b[j]));
                auto y = convolution<P>(copy(a[2 * i + 1]), copy(b[C + j]));
                x.resize(std::max(x.n, y.n));
                for (usize k = 0; k < y.n; ++k) x[k] = M::add(x[k], y[k]);
                trim(x); result[i * C + j] = std::move(x);
            }
            return result;
        }
        usize count = an + bn - 1, size = std::max<usize>(64, std::bit_ceil(count));
        Matrix left; std::array<Poly, 2 * C> right; Poly temp(size);
        const auto& ntt = convolution_detail::info<P>;
        auto forward = [&](const Poly& src, Poly& dst) {
            dst = copy(src); dst.resize(size);
            ntt.forward((convolution_detail::Vec*)dst.p, size / 8);
        };
        for (usize i = 0; i < 4; ++i) forward(a[i], left[i]);
        for (usize i = 0; i < 2 * C; ++i) forward(b[i], right[i]);
        for (usize i = 0; i < 2; ++i) for (usize j = 0; j < C; ++j) {
            auto& out = result[i * C + j]; out = copy(left[2 * i]);
            memcpy(temp.p, left[2 * i + 1].p, size * 4);
            auto* x = (convolution_detail::Vec*)out.p;
            ntt.products(x, (convolution_detail::Vec*)right[j].p, size / 8);
            ntt.products((convolution_detail::Vec*)temp.p, (convolution_detail::Vec*)right[C + j].p, size / 8);
            for (usize k = 0; k < size; ++k) out[k] = M::add(std::min(out[k], out[k] - P), std::min(temp[k], temp[k] - P));
            ntt.inverse(x, size / 8); out.n = count; trim(out);
        }
        return result;
    }
    static void step(Matrix& r, std::span<const u32> q) {
        for (usize j = 0; j < 2; ++j) {
            r[j] = subtract_product(std::move(r[j]), q, r[2 + j]);
            std::swap(r[j], r[2 + j]);
        }
    }
    // deg(a)>deg(b). Return R with (c,d)=R*(a,b), deg(d)<a.size()/2.
    static Matrix half(std::span<const u32> a, std::span<const u32> b) {
        usize middle = a.size() / 2;
        if (b.size() <= middle) return identity();
        if (a.size() <= 64) {
            Poly x = copy(a), y = copy(b); auto r = identity();
            while (y.n > middle) {
                auto [q, rem] = polynomial_divmod<P>(std::move(x), y);
                step(r, q); x = std::move(y); y = std::move(rem);
            }
            return r;
        }
        auto r = half(a.subspan(middle), b.subspan(middle));
        std::array<Poly, 2> values{copy(a), copy(b)};
        auto v = multiply<1>(r, values);
        if (v[1].n <= middle) return r;
        auto [q, rem] = polynomial_divmod<P>(std::move(v[0]), v[1]);
        step(r, q);
        usize shift = 2 * middle - (v[1].n - 1);
        auto top = [](const Poly& p, usize shift) -> std::span<const u32> {
            return shift < p.n ? std::span<const u32>(p.p + shift, p.n - shift) : std::span<const u32>{};
        };
        auto s = half(top(v[1], shift), top(rem, shift));
        return multiply<2>(s, r);
    }
};

template<u32 P = 998244353>
Buffer<u32> polynomial_gcd(Buffer<u32> a, Buffer<u32> b) {
    using E = PolynomialEuclid<P>;
    E::trim(a); E::trim(b);
    if (a.n < b.n) std::swap(a, b);
    while (b.n) {
        if (a.n > b.n && a.n > 64 && b.n > a.n / 2) {
            auto r = E::half(a, b);
            std::array<Buffer<u32>, 2> v{std::move(a), std::move(b)};
            v = E::template multiply<1>(r, v); a = std::move(v[0]); b = std::move(v[1]);
            if (!b.n) break;
        }
        auto rem = polynomial_divmod<P>(std::move(a), b).second;
        a = std::move(b); b = std::move(rem);
    }
    if (a.n) fps_scale<P>(a, Mod<P>::pow(a[a.n - 1], P - 2));
    return a;
}

// Inverse modulo an arbitrary nonzero polynomial; nullopt iff no inverse exists.
template<u32 P = 998244353>
std::optional<Buffer<u32>> polynomial_inv_mod(std::span<const u32> f, std::span<const u32> modulus) {
    using E = PolynomialEuclid<P>;
    if (modulus.size() == 1) return Buffer<u32>{};
    std::array<Buffer<u32>, 2> v{E::copy(modulus), E::copy(f)}, coefficient;
    if (v[1].n >= v[0].n) v[1] = polynomial_divmod<P>(std::move(v[1]), v[0]).second;
    coefficient[1] = Buffer<u32>(1); coefficient[1][0] = 1;
    while (v[1].n) {
        auto r = E::half(v[0], v[1]);
        v = E::template multiply<1>(r, v);
        coefficient = E::template multiply<1>(r, coefficient);
        if (!v[1].n) break;
        auto [q, rem] = polynomial_divmod<P>(std::move(v[0]), v[1]);
        coefficient[0] = E::subtract_product(std::move(coefficient[0]), q, coefficient[1]);
        std::swap(coefficient[0], coefficient[1]); v[0] = std::move(v[1]); v[1] = std::move(rem);
    }
    if (v[0].n != 1) return std::nullopt;
    fps_scale<P>(coefficient[0], Mod<P>::pow(v[0][0], P - 2));
    return std::move(coefficient[0]);
}
}
