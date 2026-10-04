#pragma once
#include <toy/mod.h>

namespace toy {
template <u32 P = 998244353>
struct Affine {
    u32 a = 1, b = 0;
    u32 operator()(u32 x) const { return (u64(a) * x + b) % P; }
};
template <u32 P = 998244353>
struct ComposeAffine {
    Affine<P> operator()(Affine<P> first, Affine<P> second) const {
        return {Mod<P>::mul(second.a, first.a), u32((u64(second.a) * first.b + second.b) % P)};
    }
};
} // namespace toy
