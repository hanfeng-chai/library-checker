#pragma once
#include <toy/mod.h>

namespace toy {
// p is prime. Trial division factors p-1; sufficient for a 32-bit modulus.
inline u32 primitive_root(u32 p) {
    array<u32, 16> factors;
    int count = 0;
    u32 remaining = p - 1;
    for (u32 d = 2; u64(d) * d <= remaining; ++d) if (remaining % d == 0) {
        factors[count++] = d;
        do remaining /= d; while (remaining % d == 0);
    }
    if (remaining > 1) factors[count++] = remaining;
    Barrett mod(p);
    for (u32 g = 1;; ++g) {
        bool ok = true;
        for (int i = 0; i < count; ++i) if (mod.pow(g, (p - 1) / factors[i]) == 1) { ok = false; break; }
        if (ok) return g;
    }
}
}
