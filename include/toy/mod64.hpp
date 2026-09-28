#pragma once

#include <bits/extc++.h>

namespace toy {

using i8 = int8_t;
using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;

class Montgomery64 {
    u64 mod_;
    u64 neg_inv_;
    u64 r2_;

public:
    explicit Montgomery64(u64 mod) : mod_(mod), neg_inv_(mod) {
        for (int i = 0; i < 6; ++i) neg_inv_ *= 2 - mod * neg_inv_;
        neg_inv_ = -neg_inv_;
        u64 r = (u128(1) << 64) % mod_;
        r2_ = (u128)r * r % mod_;
    }

    [[nodiscard]] u64 mod() const { return mod_; }

    [[nodiscard, gnu::always_inline]] u64 reduce(u128 x) const {
        u64 q = (u64)x * neg_inv_;
        u64 result = (x + (u128)q * mod_) >> 64;
        return result >= mod_ ? result - mod_ : result;
    }

    [[nodiscard, gnu::always_inline]] u64 init(u64 x) const {
        return reduce((u128)(x % mod_) * r2_);
    }

    [[nodiscard, gnu::always_inline]] u64 mul(u64 a, u64 b) const {
        return reduce((u128)a * b);
    }

    [[nodiscard]] u64 one() const { return init(1); }
    [[nodiscard]] u64 value(u64 x) const { return reduce(x); }

    [[nodiscard]] u64 pow(u64 base, u64 exponent) const {
        u64 x = init(base), result = one();
        while (exponent) {
            if (exponent & 1) result = mul(result, x);
            x = mul(x, x);
            exponent >>= 1;
        }
        return result;
    }
};

} // namespace toy
