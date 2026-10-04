#pragma once
#include <toy/polynomial_mod.h>

namespace toy {
// Distinct roots in F_P, in arbitrary order. f is nonzero, P an odd prime.
template <u32 P = 998244353>
Buffer<u32> polynomial_roots(Buffer<u32> f, u64 seed = 712367821) {
    using M = Mod<P>;
    using E = PolynomialEuclid<P>;
    E::trim(f);
    Buffer<u32> answer(0, f.n - 1);
    auto append = [&](u32 x) { answer.p[answer.n++] = x; };
    usize first = 0;
    while (!f[first]) ++first;
    if (first) {
        append(0);
        memmove(f.p, f.p + first, (f.n - first) * 4);
        f.n -= first;
    }
    if (f.n <= 1) return answer;
    fps_scale<P>(f, M::pow(f[f.n - 1], P - 2));
    PolynomialMod<P> mod(f);
    auto h = mod.linear_power(0, P - 1);
    if (!h.n) h.resize(1);
    h[0] = M::sub(h[0], 1);
    f = polynomial_gcd<P>(std::move(f), std::move(h));
    auto split = [&](auto &&self, Buffer<u32> g) -> void {
        if (g.n <= 1) return;
        if (g.n == 2) {
            append(M::sub(0, g[0]));
            return;
        }
        if (g.n == 3) {
            u32 root = *mod_sqrt<P>(M::sub(M::mul(g[1], g[1]), M::mul(4, g[0])));
            append(M::mul(M::sub(root, g[1]), (P + 1) / 2));
            append(M::mul(M::sub(M::sub(0, root), g[1]), (P + 1) / 2));
            return;
        }
        PolynomialMod<P> ring(g);
        for (;;) {
            seed ^= seed << 13;
            seed ^= seed >> 7;
            seed ^= seed << 17;
            auto v = ring.linear_power(seed % P, (P - 1) / 2);
            if (!v.n) v.resize(1);
            v[0] = M::sub(v[0], 1);
            auto factor = polynomial_gcd<P>(E::copy(g), std::move(v));
            if (factor.n <= 1 || factor.n == g.n) continue;
            auto other = polynomial_divmod<P>(std::move(g), factor).first;
            self(self, std::move(factor));
            self(self, std::move(other));
            return;
        }
    };
    split(split, std::move(f));
    return answer;
}
} // namespace toy
