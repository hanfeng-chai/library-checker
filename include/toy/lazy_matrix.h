#pragma once
#include <toy/buffer.h>
#include <toy/mod.h>
namespace toy {
// Canonical pivot rows, lazy u64 destination rows. Pairing pivots and target
// rows shares source loads and destination traffic; bounds stay below 16P².
template <u32 P = 998244353>
struct LazyMatrix {
    u32 n, width, stride;
    Buffer<u64> data;
    Buffer<u64 *> row;
    Buffer<u8> count;
    LazyMatrix(u32 n, u32 width, std::span<const u32> input)
        : n(n), width(width), stride((width + 7) & ~7u), data(usize(n) * stride), row(n), count(n) {
        std::fill(count.p, count.p + n, 0);
        for (u32 i = 0; i < n; ++i) {
            row[i] = data.p + usize(i) * stride;
            for (u32 j = 0; j < width; ++j) row[i][j] = input[usize(i) * width + j];
        }
    }
    void swap_rows(u32 a, u32 b) {
        std::swap(row[a], row[b]);
        std::swap(count[a], count[b]);
    }
    u32 get(u32 i, u32 j) const { return row[i][j] % P; }
    template <bool Pair = false, bool Two = false>
    void add(u32 i, const u64 *__restrict x, const u64 *__restrict y, u32 a, u32 b, u32 c, u32 d,
             u32 first) {
        u64 *__restrict dst = row[i];
        u64 *__restrict dst2 = Pair ? row[i + 1] : nullptr;
        u32 j = first;
        auto scalar = [&](u32 j) {
            dst[j] += u64(a) * x[j];
            if constexpr (Two) dst[j] += u64(b) * y[j];
            if constexpr (Pair) {
                dst2[j] += u64(c) * x[j];
                if constexpr (Two) dst2[j] += u64(d) * y[j];
            }
        };
        for (; j < width && j % 4; ++j) scalar(j);
        auto sa = _mm256_set1_epi64x(a), sb = _mm256_set1_epi64x(b), sc = _mm256_set1_epi64x(c),
             sd = _mm256_set1_epi64x(d);
        for (; j + 4 <= width; j += 4) {
            auto u = _mm256_load_si256((const __m256i *)(x + j));
            __m256i v;
            if constexpr (Two) v = _mm256_load_si256((const __m256i *)(y + j));
            auto z = _mm256_add_epi64(_mm256_load_si256((const __m256i *)(dst + j)),
                                      _mm256_mul_epu32(u, sa));
            if constexpr (Two) z = _mm256_add_epi64(z, _mm256_mul_epu32(v, sb));
            _mm256_store_si256((__m256i *)(dst + j), z);
            if constexpr (Pair) {
                auto z = _mm256_add_epi64(_mm256_load_si256((const __m256i *)(dst2 + j)),
                                          _mm256_mul_epu32(u, sc));
                if constexpr (Two) z = _mm256_add_epi64(z, _mm256_mul_epu32(v, sd));
                _mm256_store_si256((__m256i *)(dst2 + j), z);
            }
        }
        for (; j < width; ++j) scalar(j);
        for (u32 r = i; r <= i + Pair; ++r)
            if ((count[r] += Two ? 2 : 1) >= 8) {
                u64 *dst = row[r];
                constexpr u64 bound_value = 8ull * P * P;
                auto sign = _mm256_set1_epi64x(INT64_MIN), bound = _mm256_set1_epi64x(bound_value),
                     threshold = _mm256_set1_epi64x((bound_value - 1) ^ u64(INT64_MIN));
                j = first;
                for (; j < width && j % 4; ++j)
                    if (dst[j] >= bound_value) dst[j] -= bound_value;
                for (; j + 4 <= width; j += 4) {
                    auto z = _mm256_load_si256((const __m256i *)(dst + j)),
                         mask = _mm256_cmpgt_epi64(_mm256_xor_si256(z, sign), threshold);
                    _mm256_store_si256((__m256i *)(dst + j),
                                       _mm256_sub_epi64(z, _mm256_and_si256(mask, bound)));
                }
                for (; j < width; ++j)
                    if (dst[j] >= bound_value) dst[j] -= bound_value;
                count[r] -= 8;
            }
    }
};
} // namespace toy
