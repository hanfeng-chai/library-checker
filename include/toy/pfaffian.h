#pragma once
#include <toy/buffer.h>
#include <toy/mod.h>
namespace toy {
// Two-index Schur complements of a skew-symmetric matrix. Only the upper
// triangle is updated; paired rows share four-wide lazy u64 accumulation.
template <u32 P = 998244353>
u32 pfaffian(u32 n, std::span<const u32> input) {
    using M = Mod<P>;
    u32 stride = (n + 7) & ~7u;
    Buffer<u64> a(usize(n) * stride);
    Buffer<u8> count(n);
    std::fill(count.p, count.p + n, 0);
    for (u32 i = 0; i < n; ++i)
        for (u32 j = i + 1; j < n; ++j) a[usize(i) * stride + j] = input[usize(i) * n + j];
    auto row = [&](u32 i) { return a.p + usize(i) * stride; };
    auto get = [&](u32 i, u32 j) { return i < j ? u32(row(i)[j] % P) : M::sub(0, row(j)[i] % P); };
    auto set = [&](u32 i, u32 j, u32 x) {
        if (i < j)
            row(i)[j] = x;
        else
            row(j)[i] = M::sub(0, x);
    };
    u32 answer = 1;
    constexpr u64 limit = 8ull * P * P;
    auto sign = _mm256_set1_epi64x(INT64_MIN), bound = _mm256_set1_epi64x(limit),
         threshold = _mm256_set1_epi64x((limit - 1) ^ u64(INT64_MIN));
    for (u32 k = 0; k < n; k += 2) {
        u32 p = k + 1;
        while (p < n && !get(k, p)) ++p;
        if (p == n) return 0;
        if (p != k + 1) {
            u32 q = k + 1, value = get(q, p);
            for (u32 i = 0; i < n; ++i)
                if (i != p && i != q) {
                    u32 x = get(i, p), y = get(i, q);
                    set(i, p, y);
                    set(i, q, x);
                }
            set(q, p, M::sub(0, value));
            count[q] = count[p] = 0;
            answer = M::sub(0, answer);
        }
        u64 *first = row(k), *second = row(k + 1);
        for (u32 j = k + 1; j < n; ++j) first[j] %= P;
        for (u32 j = k + 2; j < n; ++j) second[j] %= P;
        u32 pivot = first[k + 1], inverse = M::pow(pivot, P - 2);
        answer = M::mul(answer, pivot);
        for (u32 i = k + 2; i < n; ++i) {
            u32 f = M::mul(second[i], inverse), g = M::sub(0, M::mul(first[i], inverse));
            if (!(f | g)) continue;
            u64 *dst = row(i);
            u32 j = i + 1;
            for (; j < n && j % 4; ++j) dst[j] += u64(f) * first[j] + u64(g) * second[j];
            auto x = _mm256_set1_epi64x(f), y = _mm256_set1_epi64x(g);
            for (; j + 4 <= n; j += 4) {
                auto value = _mm256_load_si256((const __m256i *)(dst + j));
                value = _mm256_add_epi64(
                    value, _mm256_mul_epu32(_mm256_load_si256((const __m256i *)(first + j)), x));
                value = _mm256_add_epi64(
                    value, _mm256_mul_epu32(_mm256_load_si256((const __m256i *)(second + j)), y));
                _mm256_store_si256((__m256i *)(dst + j), value);
            }
            for (; j < n; ++j) dst[j] += u64(f) * first[j] + u64(g) * second[j];
            if ((count[i] += 2) == 8) {
                j = i + 1;
                for (; j < n && j % 4; ++j)
                    if (dst[j] >= limit) dst[j] -= limit;
                for (; j + 4 <= n; j += 4) {
                    auto value = _mm256_load_si256((const __m256i *)(dst + j)),
                         mask = _mm256_cmpgt_epi64(_mm256_xor_si256(value, sign), threshold);
                    _mm256_store_si256((__m256i *)(dst + j),
                                       _mm256_sub_epi64(value, _mm256_and_si256(mask, bound)));
                }
                for (; j < n; ++j)
                    if (dst[j] >= limit) dst[j] -= limit;
                count[i] = 0;
            }
        }
    }
    return answer;
}
} // namespace toy
