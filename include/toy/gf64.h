#pragma once
#include <toy/common.h>

namespace toy {
// GF(2^64), polynomial basis modulo x^64+x^4+x^3+x+1. Addition is XOR.
// PCLMUL is required; keep the target local to field arithmetic.
[[gnu::target("pclmul")]] inline u64 gf64_mul(u64 a, u64 b) {
    auto z = _mm_clmulepi64_si128(_mm_cvtsi64_si128(a), _mm_cvtsi64_si128(b), 0);
    auto t = _mm_clmulepi64_si128(_mm_srli_si128(z, 8), _mm_cvtsi64_si128(27), 0);
    // Fold the high half with x^64=27; at most four bits remain above degree 63.
    u64 high = _mm_extract_epi64(t, 1);
    return u64(_mm_cvtsi128_si64(_mm_xor_si128(z, t))) ^ high ^ (high << 1) ^ (high << 3) ^ (high << 4);
}

namespace gf64_detail {
constexpr u64 slow_mul(u64 a, u64 b) {
    u64 r = 0;
    for (; b; b >>= 1, a = (a << 1) ^ (27 & -i64(a >> 63))) if (b & 1) r ^= a;
    return r;
}
// x^61 has trace one. Its Artin-Schreier chain ends in 1, then 0;
// each suffix is a Cantor basis, so no runtime inversions or random basis search.
inline constexpr auto chain = [] {
    array<u64, 64> a{};
    u64 x = u64(1) << 61;
    for (auto& v : a) v = x, x = slow_mul(x, x) ^ x;
    return a;
}();
static_assert(chain[63] == 1);
}
}
