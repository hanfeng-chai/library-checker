#pragma once
#include <toy/buffer.h>

namespace toy {
// Nonempty half-open queries. A 32-element monotone stack fits in one word;
// cross-block queries combine endpoint minima and a sparse table of blocks.
template<class T> struct RMQ {
    Buffer<T> values, prefix, suffix, table;
    Buffer<u32> masks;
    usize blocks;
    explicit RMQ(Buffer<T> a) : values(std::move(a)), prefix(values.n), suffix(values.n), masks(values.n), blocks((values.n + 31) / 32) {
        table = Buffer<T>(blocks * bit_width(blocks));
        for (usize b = 0; b < blocks; ++b) {
            usize first = 32 * b, last = std::min(first + 32, values.n); u32 stack = 0;
            T low = numeric_limits<T>::max();
            for (usize i = first; i < last; ++i) {
                while (stack && values[first + 31 - countl_zero(stack)] >= values[i]) stack &= ~(1u << (31 - countl_zero(stack)));
                stack |= 1u << (i - first); masks[i] = stack;
                prefix[i] = low = std::min(low, values[i]);
            }
            table[b] = low; low = numeric_limits<T>::max();
            for (usize i = last; i-- > first;) suffix[i] = low = std::min(low, values[i]);
        }
        for (usize k = 1; (usize(1) << k) <= blocks; ++k)
            for (usize i = 0; i + (usize(1) << k) <= blocks; ++i)
                table[k * blocks + i] = std::min(table[(k - 1) * blocks + i], table[(k - 1) * blocks + i + (usize(1) << (k - 1))]);
    }
    T min(usize l, usize r) const {
        usize first = l / 32, last = (r - 1) / 32;
        if (first == last) return values[32 * first + countr_zero(masks[r - 1] & (~u32(0) << (l & 31)))];
        T answer = std::min(suffix[l], prefix[r - 1]);
        if (++first < last) {
            usize k = bit_width(last - first) - 1;
            answer = std::min(answer, std::min(table[k * blocks + first], table[k * blocks + last - (usize(1) << k)]));
        }
        return answer;
    }
};
// u32 specialization: SIMD prefix/suffix scans, 16-element blocks.
template<> struct RMQ<u32> {
    Buffer<u32> values, prefix, suffix, table;
    usize blocks, stride;
    explicit RMQ(Buffer<u32> a) : values(std::move(a)), blocks((values.n + 15) / 16), stride(blocks + 1) {
        usize old = values.n; values.resize(16 * blocks); fill(values.p + old, values.p + values.n, ~u32(0));
        prefix = Buffer<u32>(values.n); suffix = Buffer<u32>(values.n);
        table = Buffer<u32>((bit_width(blocks) + 1) * stride); fill(table.p, table.p + table.n, ~u32(0));
        auto p1 = _mm256_setr_epi32(0,0,1,2,3,4,5,6), p2 = _mm256_setr_epi32(0,0,0,1,2,3,4,5), p4 = _mm256_setr_epi32(0,0,0,0,0,1,2,3);
        auto s1 = _mm256_setr_epi32(1,2,3,4,5,6,7,7), s2 = _mm256_setr_epi32(2,3,4,5,6,7,7,7), s4 = _mm256_setr_epi32(4,5,6,7,7,7,7,7);
        for (usize b = 0; b < blocks; ++b) {
            auto before = _mm256_set1_epi32(-1), after = before;
            // Doubling shifts combine minima over 2,4,8 lanes. Clamping the
            // shifted indices to an endpoint is safe for the idempotent minimum.
            for (usize j = 0; j < 16; j += 8) {
                auto x = _mm256_loadu_si256((const __m256i*)(values.p + 16 * b + j));
                x = _mm256_min_epu32(x, _mm256_permutevar8x32_epi32(x, p1));
                x = _mm256_min_epu32(x, _mm256_permutevar8x32_epi32(x, p2));
                x = _mm256_min_epu32(x, _mm256_permutevar8x32_epi32(x, p4));
                x = _mm256_min_epu32(x, before); _mm256_storeu_si256((__m256i*)(prefix.p + 16 * b + j), x);
                before = _mm256_permutevar8x32_epi32(x, _mm256_set1_epi32(7));
            }
            for (usize j = 16; j; j -= 8) {
                auto x = _mm256_loadu_si256((const __m256i*)(values.p + 16 * b + j - 8));
                x = _mm256_min_epu32(x, _mm256_permutevar8x32_epi32(x, s1));
                x = _mm256_min_epu32(x, _mm256_permutevar8x32_epi32(x, s2));
                x = _mm256_min_epu32(x, _mm256_permutevar8x32_epi32(x, s4));
                x = _mm256_min_epu32(x, after); _mm256_storeu_si256((__m256i*)(suffix.p + 16 * b + j - 8), x);
                after = _mm256_permutevar8x32_epi32(x, _mm256_setzero_si256());
            }
            table[stride + b] = suffix[16 * b];
        }
        for (usize k = 2; k <= bit_width(blocks); ++k)
            for (usize i = 0; i + (usize(1) << (k - 1)) <= blocks; ++i)
                table[k * stride + i] = std::min(table[(k - 1) * stride + i], table[(k - 1) * stride + i + (usize(1) << (k - 2))]);
    }
    static u32 reduce(__m256i x) {
        auto v = _mm_min_epu32(_mm256_castsi256_si128(x), _mm256_extracti128_si256(x, 1));
        v = _mm_min_epu32(v, _mm_shuffle_epi32(v, 0x4e)); v = _mm_min_epu32(v, _mm_shuffle_epi32(v, 0xb1));
        return _mm_cvtsi128_si32(v);
    }
    u32 min(usize l, usize r) const {
        usize first = l / 16, last = (r - 1) / 16;
        if (first == last) {
            if (r == l + 1) return values[l];
            if (r - l > 8) return reduce(_mm256_min_epu32(_mm256_loadu_si256((const __m256i*)(values.p + l)), _mm256_loadu_si256((const __m256i*)(values.p + r - 8))));
            usize start = 16 * first + std::min<usize>(l & 15, 8);
            auto index = _mm256_setr_epi32(0,1,2,3,4,5,6,7);
            auto valid = _mm256_and_si256(_mm256_cmpgt_epi32(index, _mm256_set1_epi32(int(l - start) - 1)), _mm256_cmpgt_epi32(_mm256_set1_epi32(r - start), index));
            return reduce(_mm256_or_si256(_mm256_loadu_si256((const __m256i*)(values.p + start)), _mm256_andnot_si256(valid, _mm256_set1_epi32(-1))));
        }
        usize gap = last - first - 1, k = bit_width((gap << 1) | usize(1)) - 1, width = (usize(1) << k) / 2;
        return std::min(std::min(suffix[l], prefix[r - 1]), std::min(table[k * stride + first + 1], table[k * stride + last - width]));
    }
};

}
