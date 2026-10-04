#pragma once
#include <toy/convolution.h>
#include <toy/ntt.h>

namespace toy {
template <u32 P>
struct RadixNTT : NTT<P> {
    using M = Mod<P>;
    static constexpr u32 generator() {
        u32 g = M::mont(3, M::r2);
        while (M::pow(M::mont(g, 1), P >> 1) != P - 1) ++g;
        return M::mont(g, 1);
    }
    explicit RadixNTT(usize capacity) : NTT<P>(capacity, generator()) {}
    void forward(std::span<u32> a) const {
        if (a.size() < 64) {
            NTT<P>::forward(a);
            return;
        }
        convolution_detail::info<P>.forward((__m256i *)a.data(), a.size() / 8);
        for (usize i = 0; i < a.size(); i += 8) {
            auto x = _mm256_load_si256((const __m256i *)(a.data() + i));
            auto p = _mm256_set1_epi32(P), p2 = _mm256_set1_epi32(2 * P);
            x = _mm256_min_epu32(x, _mm256_sub_epi32(x, p2));
            _mm256_store_si256((__m256i *)(a.data() + i),
                               _mm256_min_epu32(x, _mm256_sub_epi32(x, p)));
        }
        this->template small<false>(a, this->root.p);
    }
    void inverse(std::span<u32> a) const {
        if (a.size() < 64) {
            NTT<P>::inverse(a);
            return;
        }
        this->template small<true>(a, this->inverse_root.p);
        // Partial inverse includes Montgomery R. The eight-point leaves are
        // unnormalized, so multiplying by ordinary 1/8 with mont cancels both.
        auto scale = _mm256_set1_epi32(P - (P - 1) / 8);
        for (usize i = 0; i < a.size(); i += 8)
            _mm256_store_si256((__m256i *)(a.data() + i),
                               M::mont(_mm256_load_si256((const __m256i *)(a.data() + i)), scale));
        convolution_detail::info<P>.inverse((__m256i *)a.data(), a.size() / 8);
    }
};

} // namespace toy
