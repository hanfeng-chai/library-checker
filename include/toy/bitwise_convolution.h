#pragma once
#include <toy/buffer.h>
#include <toy/mod.h>

namespace toy {
enum class Bitwise { And, Or, Xor };

namespace bitwise_detail {
template <Bitwise Op, bool Inverse, u32 P>
[[gnu::always_inline]] inline void butterfly(u32 *a, u32 *b, usize n) {
    using M = Mod<P>;
    usize i = 0;
    for (; i + 8 <= n; i += 8) {
        auto x = _mm256_loadu_si256((const __m256i *)(a + i));
        auto y = _mm256_loadu_si256((const __m256i *)(b + i));
        if constexpr (Op == Bitwise::Xor) {
            _mm256_storeu_si256((__m256i *)(a + i), M::add(x, y));
            _mm256_storeu_si256((__m256i *)(b + i), M::sub(x, y));
        } else if constexpr (Op == Bitwise::And)
            _mm256_storeu_si256((__m256i *)(a + i), Inverse ? M::sub(x, y) : M::add(x, y));
        else
            _mm256_storeu_si256((__m256i *)(b + i), Inverse ? M::sub(y, x) : M::add(y, x));
    }
    for (; i < n; ++i) {
        u32 x = a[i], y = b[i];
        if constexpr (Op == Bitwise::Xor)
            a[i] = M::add(x, y), b[i] = M::sub(x, y);
        else if constexpr (Op == Bitwise::And)
            a[i] = Inverse ? M::sub(x, y) : M::add(x, y);
        else
            b[i] = Inverse ? M::sub(y, x) : M::add(y, x);
    }
}

template <Bitwise Op, bool Inverse, u32 P, int Mask>
[[gnu::always_inline]] inline __m256i lane_step(__m256i x, __m256i y) {
    using M = Mod<P>;
    if constexpr (Op == Bitwise::Xor)
        return _mm256_blend_epi32(M::add(x, y), M::sub(y, x), Mask);
    else if constexpr (Op == Bitwise::And)
        return _mm256_blend_epi32(Inverse ? M::sub(x, y) : M::add(x, y), x, Mask);
    else
        return _mm256_blend_epi32(x, Inverse ? M::sub(x, y) : M::add(x, y), Mask);
}

template <Bitwise Op, bool Inverse, u32 P>
[[gnu::always_inline]] inline void transform(u32 *a, usize n) {
    if (n >= 8) {
        // The low three dimensions fit one vector. The masks select indices
        // with bit 0, 1, or 2 set, so no intermediate arrays are needed.
        for (usize i = 0; i < n; i += 8) {
            auto x = _mm256_loadu_si256((const __m256i *)(a + i));
            x = lane_step<Op, Inverse, P, 0xaa>(x,
                                                _mm256_shuffle_epi32(x, _MM_SHUFFLE(2, 3, 0, 1)));
            x = lane_step<Op, Inverse, P, 0xcc>(x,
                                                _mm256_shuffle_epi32(x, _MM_SHUFFLE(1, 0, 3, 2)));
            x = lane_step<Op, Inverse, P, 0xf0>(x, _mm256_permute2x128_si256(x, x, 1));
            _mm256_storeu_si256((__m256i *)(a + i), x);
        }
    }
    for (usize half = n >= 8 ? 8 : 1; half < n; half *= 2)
        for (usize i = 0; i < n; i += 2 * half)
            butterfly<Op, Inverse, P>(a + i, a + i + half, half);
}

template <Bitwise Op, u32 P>
void product(u32 *__restrict__ a, u32 *__restrict__ b, usize n) {
    if (n > 1024) {
        usize half = n / 2;
        butterfly<Op, false, P>(a, a + half, half);
        butterfly<Op, false, P>(b, b + half, half);
        product<Op, P>(a, b, half);
        product<Op, P>(a + half, b + half, half);
        butterfly<Op, true, P>(a, a + half, half);
        return;
    }
    transform<Op, false, P>(a, n);
    transform<Op, false, P>(b, n);
    usize i = 0;
    for (; i + 8 <= n; i += 8)
        _mm256_storeu_si256((__m256i *)(a + i),
                            Mod<P>::mont(_mm256_loadu_si256((const __m256i *)(a + i)),
                                         _mm256_loadu_si256((const __m256i *)(b + i))));
    for (; i < n; ++i) a[i] = Mod<P>::mont(a[i], b[i]);
    transform<Op, true, P>(a, n);
}
} // namespace bitwise_detail

// Equal power-of-two lengths; ordinary residues in [0, P). Consumes both buffers.
template <Bitwise Op, u32 P = 998244353>
Buffer<u32> bitwise_convolution(Buffer<u32> a, Buffer<u32> b) {
    using M = Mod<P>;
    if (!a.n) return a;
    // Only B enters Montgomery space. For XOR it also absorbs 1/N, allowing
    // every inverse butterfly to use plain additions and subtractions.
    u32 scale = M::r2;
    if constexpr (Op == Bitwise::Xor)
        scale = M::mul(scale, M::pow((P + 1) / 2, std::countr_zero(a.n)));
    usize i = 0;
    auto factor = _mm256_set1_epi32(scale);
    for (; i + 8 <= b.n; i += 8)
        _mm256_storeu_si256((__m256i *)(b.p + i),
                            M::mont(_mm256_loadu_si256((const __m256i *)(b.p + i)), factor));
    for (; i < b.n; ++i) b[i] = M::mont(b[i], scale);
    bitwise_detail::product<Op, P>(a.p, b.p, a.n);
    return a;
}
} // namespace toy
