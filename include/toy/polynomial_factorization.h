#pragma once
#include <toy/array.h>
#include <toy/buffer.h>

namespace toy {
struct PolynomialFactor {
    Buffer<u32> coefficients;
    u32 multiplicity;
};
struct PolynomialFactors {
    Array<PolynomialFactor> data;
    usize n = 0;
    explicit PolynomialFactors(usize capacity) : data(capacity) {}
};

// Dense small-degree arithmetic. Odd fields use Montgomery residues; F_2
// specializes the same operations to bits. The prime is chosen at runtime.
template<bool Binary>
struct PolynomialFactorizer {
    using Poly = Buffer<u32>;
    u32 p, inverse, r2, one;
    u64 state;
    PolynomialFactors result;
    explicit PolynomialFactorizer(u32 p, usize n, u64 seed) : p(p), inverse(2 + p),
        r2((-u64(p)) % p), one((u64(1) << 32) % p), state(seed), result(n) {
        for (int i = 0; i < 4; ++i) inverse *= 2 + p * inverse;
        if constexpr (Binary) one = 1;
    }
    u32 add(u32 a, u32 b) const { return std::min(a + b, a + b - p); }
    u32 sub(u32 a, u32 b) const { return std::min(a - b, a - b + p); }
    // Up to eight canonical products fit together with the correction term.
    u32 reduce(u64 a) const {
        if constexpr (Binary) return a & 1;
        u32 x = (a + u64(u32(a) * inverse) * p) >> 32;
        x = std::min(x, x - 2 * p); return std::min(x, x - p);
    }
    u32 mul(u32 a, u32 b) const { return reduce(u64(a) * b); }
    u32 encode(u32 a) const { if constexpr (Binary) return a & 1; else return mul(a, r2); }
    u32 power(u32 a, u32 e) const {
        u32 r = one;
        for (; e; e >>= 1, a = mul(a, a)) if (e & 1) r = mul(r, a);
        return r;
    }
    static void trim(Poly& a) { while (a.n && !a[a.n - 1]) --a.n; }
    static Poly copy(std::span<const u32> a) {
        Poly b(a.size()); if (!a.empty()) memcpy(b.p, a.data(), a.size() * 4); return b;
    }
    Poly constant() const { Poly a(1); a[0] = one; return a; }
    template<bool Quotient = false>
    Poly divide(Poly a, std::span<const u32> b) const {
        trim(a);
        if (a.n < b.size()) { if constexpr (Quotient) return {}; else return a; }
        usize count = a.n - b.size() + 1;
        Poly q(Quotient ? count : 0); u32 inv = power(b.back(), p - 2);
        for (usize i = count; i--;) {
            u32 value = mul(a[i + b.size() - 1], inv);
            if constexpr (Quotient) q[i] = value;
            for (usize j = 0; j + 1 < b.size(); ++j) a[i + j] = sub(a[i + j], mul(value, b[j]));
        }
        if constexpr (Quotient) { trim(q); return q; }
        else { a.n = b.size() - 1; trim(a); return a; }
    }
    Poly multiply(std::span<const u32> a, std::span<const u32> b) const {
        if (a.empty() || b.empty()) return {};
        Poly c(a.size() + b.size() - 1);
        for (usize k = 0; k < c.n; ++k) {
            u32 value = 0; usize end = std::min(a.size(), k + 1);
            for (usize first = k < b.size() ? 0 : k - b.size() + 1; first < end; first += 8) {
                u64 sum = 0;
                for (usize i = first; i < std::min(first + 8, end); ++i) sum += u64(a[i]) * b[k - i];
                value = add(value, reduce(sum));
            }
            c[k] = value;
        }
        trim(c); return c;
    }
    Poly gcd(Poly a, Poly b) const {
        trim(a); trim(b);
        while (b.n) { auto r = divide(std::move(a), b); a = std::move(b); b = std::move(r); }
        if (a.n) { u32 inv = power(a[a.n - 1], p - 2); for (usize i = 0; i < a.n; ++i) a[i] = mul(a[i], inv); }
        return a;
    }
    Poly powmod(Poly a, u32 e, std::span<const u32> f) const {
        a = divide(std::move(a), f); auto r = constant();
        for (; e; e >>= 1) {
            if (e & 1) r = divide(multiply(r, a), f);
            if (e > 1) a = divide(multiply(a, a), f);
        }
        return r;
    }
    void append(Poly f, u32 multiplicity) {
        for (usize i = 0; i < f.n; ++i) f[i] = mul(f[i], 1);
        result.data[result.n++] = {std::move(f), multiplicity};
    }
    void factor_squarefree(Poly f, u32 multiplicity) {
        usize n = f.n - 1;
        if (!n) return;
        if (n == 1) { append(std::move(f), multiplicity); return; }
        Poly x(2); x[0] = 0; x[1] = one;
        auto xp = powmod(copy(x), p, f), current = constant();
        Buffer<u32> matrix(n * n);
        // Store the Frobenius map column powers transposed: each output
        // coefficient becomes a contiguous dot product, even after f shrinks.
        for (usize i = 0; i < n; ++i) {
            for (usize j = 0; j < n; ++j) matrix[j * n + i] = j < current.n ? current[j] : 0;
            if (i + 1 < n) current = divide(multiply(current, xp), f);
        }
        auto frobenius = [&](std::span<const u32> a, std::span<const u32> modulus) {
            Poly b(n);
            for (usize j = 0; j < n; ++j) {
                u32 value = 0;
                for (usize first = 0; first < a.size(); first += 8) {
                    u64 sum = 0;
                    for (usize i = first; i < std::min(first + 8, a.size()); ++i) sum += u64(a[i]) * matrix[j * n + i];
                    value = add(value, reduce(sum));
                }
                b[j] = value;
            }
            return divide(std::move(b), modulus);
        };
        auto equal_degree = [&](auto&& self, Poly g, usize degree) -> void {
            if (g.n == degree + 1) { append(std::move(g), multiplicity); return; }
            for (;;) {
                Poly a(g.n - 1);
                for (usize i = 0; i < a.n; ++i) {
                    state ^= state << 13; state ^= state >> 7; state ^= state << 17;
                    a[i] = encode(state % p);
                }
                auto b = Binary ? copy(a) : powmod(std::move(a), (p - 1) / 2, g);
                auto h = copy(b);
                for (usize i = 1; i < degree; ++i) {
                    h = frobenius(h, g);
                    if constexpr (Binary) {
                        h.resize(std::max(h.n, b.n));
                        for (usize j = 0; j < b.n; ++j) h[j] = add(h[j], b[j]);
                    } else h = divide(multiply(h, b), g);
                }
                if constexpr (!Binary) { if (!h.n) h.resize(1); h[0] = sub(h[0], one); }
                auto part = gcd(copy(g), std::move(h));
                if (part.n <= 1 || part.n == g.n) continue;
                auto other = divide<true>(std::move(g), part);
                self(self, std::move(part), degree); self(self, std::move(other), degree); return;
            }
        };
        current = std::move(x);
        for (usize degree = 1; 2 * degree < f.n; ++degree) {
            current = frobenius(current, f);
            auto difference = copy(current); difference.resize(std::max<usize>(2, difference.n));
            difference[1] = sub(difference[1], one);
            auto part = gcd(copy(f), std::move(difference));
            if (part.n <= 1) continue;
            f = divide<true>(std::move(f), part);
            equal_degree(equal_degree, std::move(part), degree);
            if (f.n > 1) current = divide(std::move(current), f);
        }
        if (f.n > 1) append(std::move(f), multiplicity);
    }
    void squarefree(Poly f, u32 multiplicity = 1) {
        if (f.n <= 1) return;
        Poly derivative(f.n - 1);
        for (usize i = 1; i < f.n; ++i) derivative[i - 1] = mul(f[i], encode(i % p));
        auto repeated = gcd(copy(f), std::move(derivative));
        auto distinct = divide<true>(std::move(f), repeated);
        for (u32 i = 1; distinct.n > 1; ++i) {
            auto common = gcd(copy(distinct), copy(repeated));
            factor_squarefree(divide<true>(std::move(distinct), common), multiplicity * i);
            distinct = copy(common); repeated = divide<true>(std::move(repeated), common);
        }
        if (repeated.n > 1) {
            Poly root((repeated.n - 1) / p + 1);
            for (usize i = 0; i < root.n; ++i) root[i] = repeated[i * p];
            squarefree(std::move(root), multiplicity * p);
        }
    }
};

// Monic input over a runtime prime 2<=p<2^30. Best suited to small degrees.
inline PolynomialFactors polynomial_factorize(std::span<const u32> f, u32 p, u64 seed = 8213197421) {
    auto solve = [&]<bool Binary>() {
        PolynomialFactorizer<Binary> solver(p, f.size() - 1, seed);
        Buffer<u32> a(f.size()); for (usize i = 0; i < a.n; ++i) a[i] = solver.encode(f[i]);
        solver.squarefree(std::move(a)); return std::move(solver.result);
    };
    return p == 2 ? solve.template operator()<true>() : solve.template operator()<false>();
}
}
