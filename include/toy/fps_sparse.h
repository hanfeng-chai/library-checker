#pragma once
#include <toy/fps.h>
#include <toy/mod_sqrt.h>

namespace toy {
struct SparseTerm { u32 degree, coefficient; };
// Sorted nonzero (degree, coefficient) pairs, starting at degree zero.
template<u32 P = 998244353>
Buffer<u32> fps_inv_sparse(std::span<const SparseTerm> terms, usize n) {
    using M = Mod<P>;
    Buffer<u32> g(n);
    if (!n) return g;
    std::fill(g.p, g.p + n, 0u);
    g[0] = M::pow(terms[0].coefficient, P - 2);
    if (terms.size() == 1) return g;
    Buffer<SparseTerm> next(terms.size() - 1);
    for (usize j = 1; j < terms.size(); ++j)
        next[j - 1] = {terms[j].degree, M::sub(0, M::mul(terms[j].coefficient, g[0]))};
    // Forward substitution only propagates nonzero values. Large random gaps
    // often leave most output coefficients zero, so scanning all K terms per
    // output would do much more work than following their actual contributions.
    for (usize i = 0; i < n; ++i) if (u32 value = g[i]) {
        for (auto [degree, coefficient] : std::span(next.p, next.n)) {
            if (degree >= n - i) break;
            g[i + degree] = M::add(g[i + degree], M::mul(value, coefficient));
        }
    }
    return g;
}

// exp(f) mod x^n, f[0]=0 (omitted), n<P.
template<u32 P = 998244353>
Buffer<u32> fps_exp_sparse(std::span<const SparseTerm> terms, usize n) {
    using M = Mod<P>;
    Buffer<u32> g(n);
    if (!n) return g;
    std::fill(g.p, g.p + n, 0u); g[0] = 1;
    if (terms.empty()) return g;
    auto inverse = inverse_numbers<P>(n - 1);
    Buffer<SparseTerm> next(terms.size());
    for (usize j = 0; j < terms.size(); ++j)
        next[j] = {terms[j].degree, M::mul(terms[j].degree, terms[j].coefficient)};
    // g'=f'g: accumulate i*g[i], then divide only when its predecessors are done.
    for (usize i = 0; i < n; ++i) if (u32 value = g[i]) {
        if (i) g[i] = value = M::mul(value, inverse[i]);
        for (auto [degree, coefficient] : std::span(next.p, next.n)) {
            if (degree >= n - i) break;
            g[i + degree] = M::add(g[i + degree], M::mul(value, coefficient));
        }
    }
    return g;
}

// log(f) mod x^n, terms start with (0,1), n<P.
template<u32 P = 998244353>
Buffer<u32> fps_log_sparse(std::span<const SparseTerm> terms, usize n) {
    using M = Mod<P>;
    Buffer<u32> g(n);
    if (!n) return g;
    std::fill(g.p, g.p + n, 0u);
    if (terms.size() == 1) return g;
    Buffer<SparseTerm> next(terms.size() - 1);
    for (usize j = 1; j < terms.size(); ++j) {
        auto [degree, coefficient] = terms[j];
        next[j - 1] = {degree, M::sub(0, coefficient)};
        if (degree < n) g[degree] = M::mul(degree, coefficient);
    }
    // First solve f*(x log(f)')=x*f', then divide coefficient i by i.
    for (usize i = 1; i < n; ++i) if (u32 value = g[i]) {
        for (auto [degree, coefficient] : std::span(next.p, next.n)) {
            if (degree >= n - i) break;
            g[i + degree] = M::add(g[i + degree], M::mul(value, coefficient));
        }
    }
    auto inverse = inverse_numbers<P>(n - 1);
    for (usize i = 1; i < n; ++i) g[i] = M::mul(g[i], inverse[i]);
    return g;
}

namespace fps_sparse_detail {
// The caller zero-fills g. Strip the leading power of x from the term degrees.
// f*g'=power*f'*g gives (i+d)*g[i+d] += (power*d-i)*f[d]/f[0]*g[i].
template<u32 P>
void power(std::span<const SparseTerm> terms, std::span<u32> g, u32 exponent, u32 constant) {
    using M = Mod<P>;
    if (g.empty()) return;
    g[0] = constant;
    if (!exponent || terms.size() == 1) return;
    auto inverse = inverse_numbers<P>(g.size() - 1);
    struct Weight { u32 degree, coefficient, factor; };
    Buffer<Weight> next(terms.size() - 1);
    u32 inv0 = M::pow(terms[0].coefficient, P - 2);
    for (usize j = 1; j < terms.size(); ++j) {
        u32 d = terms[j].degree - terms[0].degree;
        next[j - 1] = {d, M::mul(terms[j].coefficient, inv0), M::mul(exponent, d)};
    }
    if (next.n <= 16 && next[next.n - 1].degree <= 64) {
        // For short lags the output is usually dense. Keep ((k+1)*d-i)*a[d]
        // as an incrementally updated coefficient and reduce the dot only once.
        // At most 16 products of residues below 2^30 fit in u64.
        for (auto& t : std::span(next.p, next.n)) t.factor = M::mul(M::add(t.factor, t.degree), t.coefficient);
        for (usize i = 1; i < g.size(); ++i) {
            u64 sum = 0;
            for (auto& t : std::span(next.p, next.n)) {
                t.factor = M::sub(t.factor, t.coefficient);
                if (t.degree <= i) sum += u64(t.factor) * g[i - t.degree];
            }
            g[i] = M::mul(sum % P, inverse[i]);
        }
        return;
    }
    for (usize i = 0; i < g.size(); ++i) if (u32 value = g[i]) {
        if (i) g[i] = value = M::mul(value, inverse[i]);
        for (auto [degree, coefficient, factor] : std::span(next.p, next.n)) {
            if (degree >= g.size() - i) break;
            g[i + degree] = M::add(g[i + degree], M::mul(M::mul(M::sub(factor, i), coefficient), value));
        }
    }
}
}

// Nonnegative integer exponent, n<P; empty terms represent zero. 0^0=1.
template<u32 P = 998244353>
Buffer<u32> fps_pow_sparse(std::span<const SparseTerm> terms, usize n, u64 exponent) {
    using M = Mod<P>;
    Buffer<u32> g(n);
    if (!n) return g;
    std::fill(g.p, g.p + n, 0u);
    if (!exponent) { g[0] = 1; return g; }
    while (!terms.empty() && terms.back().degree >= n) terms = terms.first(terms.size() - 1);
    if (terms.empty() || u128(terms[0].degree) * exponent >= n) return g;
    if (exponent == 1) { for (auto [d, c] : terms) g[d] = c; return g; }
    usize shift = terms[0].degree * exponent;
    fps_sparse_detail::power<P>(terms, std::span(g.p + shift, n - shift), exponent % P, M::pow(terms[0].coefficient, exponent));
    return g;
}

// Choose the smaller leading root; unconstrained high coefficients are zero.
template<u32 P = 998244353>
std::optional<Buffer<u32>> fps_sqrt_sparse(std::span<const SparseTerm> terms, usize n) {
    while (!terms.empty() && terms.back().degree >= n) terms = terms.first(terms.size() - 1);
    u32 shift = terms.empty() ? 0 : terms[0].degree;
    if (shift & 1) return std::nullopt;
    auto root = mod_sqrt<P>(terms.empty() ? 0 : terms[0].coefficient);
    if (!root) return std::nullopt;
    Buffer<u32> g(n);
    if (n) std::fill(g.p, g.p + n, 0u);
    if (!terms.empty()) fps_sparse_detail::power<P>(terms, std::span(g.p + shift / 2, n - shift), (P + 1) / 2, *root);
    return g;
}
}
