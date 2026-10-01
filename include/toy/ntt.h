#pragma once
#include <toy/buffer.h>
#include <toy/primitive_root.h>

namespace toy {

// Full scalar-frequency NTT, in bit-reversed order. Lengths are powers of two
// dividing P-1, no larger than the power-of-two capacity. P is prime < 2^30.
template<u32 P = 998244353>
struct NTT {
    using M = Mod<P>;
    Buffer<u32> root, inverse_root;

    explicit NTT(usize capacity, u32 g = 0) : root(std::max<usize>(1, capacity / 2)), inverse_root(root.n) {
        root[0] = inverse_root[0] = (u64(1) << 32) % P; // Montgomery one.
        if (!g) g = primitive_root(P);
        for (usize half = 1; half < root.n; half *= 2) {
            u32 w = M::pow(g, (P - 1) / (4 * half));
            u32 iw = M::mont(M::pow(w, P - 2), M::r2);
            w = M::mont(w, M::r2);
            for (usize i = 0; i < half; ++i) {
                root[half + i] = M::mont(root[i], w);
                inverse_root[half + i] = M::mont(inverse_root[i], iw);
            }
        }
    }

    template<bool Inverse>
    [[gnu::always_inline]] static void butterfly(u32* a, u32* b, usize n, u32 root) {
        auto w = _mm256_set1_epi32(root);
        usize i = 0;
        for (; i + 8 <= n; i += 8) {
            auto x = _mm256_loadu_si256((const __m256i*)(a + i));
            auto y = _mm256_loadu_si256((const __m256i*)(b + i));
            if constexpr (!Inverse) y = M::mont(y, w);
            auto sum = M::add(x, y), diff = M::sub(x, y);
            if constexpr (Inverse) diff = M::mont(diff, w);
            _mm256_storeu_si256((__m256i*)(a + i), sum);
            _mm256_storeu_si256((__m256i*)(b + i), diff);
        }
        for (; i < n; ++i) {
            u32 x = a[i], y = b[i];
            if constexpr (!Inverse) y = M::mont(y, root);
            a[i] = M::add(x, y); b[i] = M::sub(x, y);
            if constexpr (Inverse) b[i] = M::mont(b[i], root);
        }
    }

    template<bool Inverse, int Mask>
    [[gnu::always_inline]] static __m256i step(__m256i x, __m256i y, __m256i w) {
        if constexpr (!Inverse) y = M::mont(y, w);
        auto sum = M::add(x, y), diff = M::sub(x, y);
        if constexpr (Inverse) diff = M::mont(diff, w);
        return _mm256_blend_epi32(sum, diff, Mask);
    }

    template<bool Inverse>
    void small(std::span<u32> a, const u32* roots) const {
        // Three dimensions inside each vector. Duplicate each lower/upper half
        // before the butterfly; masks restore the interleaved frequency layout.
        for (usize k = 0; k < a.size() / 8; ++k) {
            auto x = _mm256_loadu_si256((const __m256i*)(a.data() + 8 * k));
            auto four = [&](auto x) { return step<Inverse, 0xf0>(
                _mm256_permute2x128_si256(x, x, 0), _mm256_permute2x128_si256(x, x, 0x11),
                _mm256_set1_epi32(roots[k])); };
            auto two = [&](auto x) { return step<Inverse, 0xcc>(
                _mm256_shuffle_epi32(x, _MM_SHUFFLE(1, 0, 1, 0)), _mm256_shuffle_epi32(x, _MM_SHUFFLE(3, 2, 3, 2)),
                _mm256_set_m128i(_mm_set1_epi32(roots[2 * k + 1]), _mm_set1_epi32(roots[2 * k]))); };
            auto one = [&](auto x) {
                auto w = _mm256_cvtepu32_epi64(_mm_loadu_si128((const __m128i*)(roots + 4 * k)));
                return step<Inverse, 0xaa>(_mm256_shuffle_epi32(x, _MM_SHUFFLE(2, 2, 0, 0)),
                    _mm256_shuffle_epi32(x, _MM_SHUFFLE(3, 3, 1, 1)), _mm256_or_si256(w, _mm256_slli_epi64(w, 32)));
            };
            if constexpr (Inverse) x = four(two(one(x)));
            else x = one(two(four(x)));
            _mm256_storeu_si256((__m256i*)(a.data() + 8 * k), x);
        }
    }

    void forward(std::span<u32> a) const {
        usize n = a.size();
        for (usize half = n / 2; half >= (n >= 8 ? 8 : 1); half /= 2)
            for (usize i = 0, k = 0; i < n; i += 2 * half, ++k)
                butterfly<false>(a.data() + i, a.data() + i + half, half, root[k]);
        if (n >= 8) small<false>(a, root.p);
    }

    // Ordinary residues in and out; optional ordinary scale multiplies output.
    void inverse(std::span<u32> a, u32 scale = 1) const {
        usize n = a.size();
        if (n >= 8) small<true>(a, inverse_root.p);
        for (usize half = n >= 8 ? 8 : 1; half < n; half *= 2)
            for (usize i = 0, k = 0; i < n; i += 2 * half, ++k)
                butterfly<true>(a.data() + i, a.data() + i + half, half, inverse_root[k]);
        u32 factor = M::mont(M::mul(M::pow(n, P - 2), scale), M::r2);
        auto w = _mm256_set1_epi32(factor);
        usize i = 0;
        for (; i + 8 <= n; i += 8) _mm256_storeu_si256((__m256i*)(a.data() + i),
            M::mont(_mm256_loadu_si256((const __m256i*)(a.data() + i)), w));
        for (; i < n; ++i) a[i] = M::mont(a[i], factor);
    }
};
}
