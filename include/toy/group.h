#pragma once
#include <toy/mod.h>

namespace toy {
template<u32 P = 998244353> struct AdditiveGroup {
    using Value = u32;
    static constexpr Value identity() { return 0; }
    static Value multiply(Value a, Value b) { return Mod<P>::add(a, b); }
    static Value inverse(Value a) { return a ? P - a : 0; }
};
// Determinant-one matrices: inverse needs no modular exponentiation.
template<u32 P = 998244353> struct Matrix2Group {
    struct Value { u32 a, b, c, d; bool operator==(const Value&) const = default; };
    static constexpr Value identity() { return {1, 0, 0, 1}; }
    static Value multiply(Value x, Value y) {
        return {u32((u64(x.a) * y.a + u64(x.b) * y.c) % P),
                u32((u64(x.a) * y.b + u64(x.b) * y.d) % P),
                u32((u64(x.c) * y.a + u64(x.d) * y.c) % P),
                u32((u64(x.c) * y.b + u64(x.d) * y.d) % P)};
    }
    static Value inverse(Value x) { return {x.d, Mod<P>::sub(0, x.b), Mod<P>::sub(0, x.c), x.a}; }
};
}
