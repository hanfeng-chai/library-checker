#pragma once
#include <toy/mod.h>

namespace toy {
// Smaller square root in an odd prime field, or nullopt for a nonresidue.
template<u32 P = 998244353>
optional<u32> mod_sqrt(u32 x) {
    using M = Mod<P>;
    if (!x) return 0;
    if (M::pow(x, (P - 1) / 2) != 1) return nullopt;
    if constexpr (P % 4 == 3) {
        u32 r = M::pow(x, (P + 1) / 4);
        return min(r, P - r);
    }
    int s = countr_zero(P - 1);
    u32 q = (P - 1) >> s, z = 2;
    while (M::pow(z, (P - 1) / 2) == 1) ++z;
    u32 c = M::pow(z, q), r = M::pow(x, (q + 1) / 2), t = M::pow(x, q);
    while (t != 1) {
        int i = 1;
        for (u32 v = M::mul(t, t); v != 1; v = M::mul(v, v)) ++i;
        u32 b = M::pow(c, u32(1) << (s - i - 1));
        r = M::mul(r, b); c = M::mul(b, b); t = M::mul(t, c); s = i;
    }
    return min(r, P - r);
}
}
