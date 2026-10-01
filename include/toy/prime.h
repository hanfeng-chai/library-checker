#pragma once
#include <toy/buffer.h>

namespace toy {

// Odd-only sieve for a moderate bound. Bit i represents 2*i+1.
inline Buffer<u32> primes(u32 limit) {
    if (limit < 2) return {};
    usize odds = (usize(limit) + 1) / 2;
    Buffer<u64> composite((odds + 63) / 64);
    std::fill(composite.p, composite.p + composite.n, 0);
    composite[0] = 1;
    for (u32 p = 3; u64(p) * p <= limit; p += 2)
        if (!(composite[(p / 2) / 64] >> ((p / 2) % 64) & 1))
            for (usize i = usize(p) * p / 2; i < odds; i += p) composite[i / 64] |= u64(1) << (i % 64);
    Buffer<u32> result((usize(limit) + 1) / 2);
    usize count = 0;
    result[count++] = 2;
    for (usize i = 0; i < composite.n; ++i) {
        u64 bits = ~composite[i];
        if (i + 1 == composite.n && odds % 64) bits &= (u64(1) << (odds % 64)) - 1;
        for (; bits; bits &= bits - 1) result[count++] = 2 * (64 * i + std::countr_zero(bits)) + 1;
    }
    result.n = count;
    return result;
}
}
