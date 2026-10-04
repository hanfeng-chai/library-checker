#pragma once
#include <toy/montgomery.h>
namespace toy {
// The slope is Montgomery encoded; the offset and arguments stay ordinary.
template <u32 P = 998244353>
struct MontAffine {
    using R = Montgomery<P>;
    u32 a = R::one, b = 0;
    static MontAffine encode(u32 a, u32 b) { return {R::encode(a), b}; }
    u32 operator()(u32 x) const {
        u32 value = R::add(R::multiply(a, x), b);
        return std::min(value, value - P);
    }
};
template <u32 P = 998244353>
struct ComposeMontAffine {
    MontAffine<P> operator()(MontAffine<P> a, MontAffine<P> b) const {
        using R = Montgomery<P>;
        return {R::multiply(a.a, b.a), R::add(R::multiply(a.b, b.a), b.b)};
    }
};
} // namespace toy
