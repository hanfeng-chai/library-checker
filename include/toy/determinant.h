#pragma once
#include <toy/buffer.h>
#include <toy/mod.h>
namespace toy {
// Canonical entries modulo prime P. Lazy u64 accumulators avoid reducing every
// product: eight updates followed by subtracting 8P^2 keep values below 16P^2.
template <u32 P = 998244353, bool Wide = true>
u32 determinant(u32 n, std::span<const u32> input) {
    static_assert(P < (1u << 30));
    using M = Mod<P>;
    u32 answer = 1;
    if constexpr (Wide) {
        u32 stride = (n + 7) & ~7u;
        Buffer<u64> a(usize(n) * stride);
        for (u32 i = 0; i < n; ++i)
            for (u32 j = 0; j < n; ++j) a[usize(i) * stride + j] = input[usize(i) * n + j];
        Buffer<u64 *> row(n);
        Buffer<u8> count(n);
        std::fill(count.p, count.p + n, 0);
        for (u32 i = 0; i < n; ++i) row[i] = a.p + usize(i) * stride;
        constexpr u64 limit = 8ull * P * P;
        auto sign = _mm256_set1_epi64x(INT64_MIN),
             threshold = _mm256_set1_epi64x((limit - 1) ^ u64(INT64_MIN)),
             bound = _mm256_set1_epi64x(limit);
        for (u32 k = 0; k < n; k += 2) {
            u32 p = k;
            while (p < n && row[p][k] % P == 0) ++p;
            if (p == n) return 0;
            if (p != k) {
                std::swap(row[p], row[k]);
                std::swap(count[p], count[k]);
                answer = P - answer;
            }
            u64 *first = row[k];
            for (u32 j = k; j < n; ++j) first[j] %= P;
            answer = M::mul(answer, first[k]);
            if (k + 1 == n) break;
            u32 inverse = M::pow(first[k], P - 2);
            auto coefficient = [&](u64 value, u32 inv) {
                u32 x = value % P;
                return x ? M::mul(P - x, inv) : 0;
            };
            p = k + 1;
            for (; p < n; ++p)
                if ((row[p][k + 1] % P + u64(coefficient(row[p][k], inverse)) * first[k + 1]) % P)
                    break;
            if (p == n) return 0;
            if (p != k + 1) {
                std::swap(row[p], row[k + 1]);
                std::swap(count[p], count[k + 1]);
                answer = P - answer;
            }
            u64 *second = row[k + 1];
            u32 factor = coefficient(second[k], inverse);
            for (u32 j = k + 1; j < n; ++j)
                second[j] = (second[j] % P + u64(factor) * first[j]) % P;
            answer = M::mul(answer, second[k + 1]);
            u32 inverse2 = M::pow(second[k + 1], P - 2);
            // Two pivots share each destination load/store. The second scale
            // includes the first pivot's effect on column k+1.
            // Align vector accesses by also overwriting eliminated columns.
            // Those entries are never read as pivots again.
            auto update = [&]<bool Pair>(u32 i, u32 a, u32 b, u32 c, u32 d) {
                u64 *__restrict dst = row[i];
                u64 *__restrict dst2 = Pair ? row[i + 1] : nullptr;
                auto sa = _mm256_set1_epi64x(a), sb = _mm256_set1_epi64x(b),
                     sc = _mm256_set1_epi64x(c), sd = _mm256_set1_epi64x(d);
                u32 j = (k + 2) & ~3u;
                for (; j + 4 <= n; j += 4) {
                    auto u = _mm256_load_si256((const __m256i *)(first + j)),
                         v = _mm256_load_si256((const __m256i *)(second + j));
                    auto x = _mm256_load_si256((const __m256i *)(dst + j));
                    x = _mm256_add_epi64(x, _mm256_mul_epu32(u, sa));
                    x = _mm256_add_epi64(x, _mm256_mul_epu32(v, sb));
                    _mm256_store_si256((__m256i *)(dst + j), x);
                    if constexpr (Pair) {
                        auto y = _mm256_load_si256((const __m256i *)(dst2 + j));
                        y = _mm256_add_epi64(y, _mm256_mul_epu32(u, sc));
                        y = _mm256_add_epi64(y, _mm256_mul_epu32(v, sd));
                        _mm256_store_si256((__m256i *)(dst2 + j), y);
                    }
                }
                for (; j < n; ++j) {
                    dst[j] += first[j] * a + second[j] * b;
                    if constexpr (Pair) dst2[j] += first[j] * c + second[j] * d;
                }
                for (u32 r = i; r <= i + Pair; ++r)
                    if ((count[r] += 2) == 8) {
                        u64 *dst = row[r];
                        j = (k + 2) & ~3u;
                        for (; j + 4 <= n; j += 4) {
                            auto x = _mm256_load_si256((const __m256i *)(dst + j));
                            auto mask = _mm256_cmpgt_epi64(_mm256_xor_si256(x, sign), threshold);
                            _mm256_store_si256((__m256i *)(dst + j),
                                               _mm256_sub_epi64(x, _mm256_and_si256(mask, bound)));
                        }
                        for (; j < n; ++j)
                            if (dst[j] >= limit) dst[j] -= limit;
                        count[r] = 0;
                    }
            };
            for (u32 i = k + 2; i < n; i += 2) {
                u32 a = coefficient(row[i][k], inverse),
                    b = coefficient(row[i][k + 1] % P + u64(a) * first[k + 1], inverse2);
                if (i + 1 < n) {
                    u32 c = coefficient(row[i + 1][k], inverse),
                        d = coefficient(row[i + 1][k + 1] % P + u64(c) * first[k + 1], inverse2);
                    if (a | b | c | d) update.template operator()<true>(i, a, b, c, d);
                } else if (a | b)
                    update.template operator()<false>(i, a, b, 0, 0);
            }
        }
    } else {
        Buffer<u32> a(input.size());
        if (a.n) memcpy(a.p, input.data(), input.size_bytes());
        for (u32 k = 0; k < n; ++k) {
            u32 p = k;
            while (p < n && !a[usize(p) * n + k]) ++p;
            if (p == n) return 0;
            if (p != k) {
                for (u32 j = k; j < n; ++j) std::swap(a[usize(k) * n + j], a[usize(p) * n + j]);
                answer = answer ? P - answer : 0;
            }
            auto pivot = a.p + usize(k) * n;
            answer = M::mul(answer, pivot[k]);
            u32 inverse = M::pow(pivot[k], P - 2);
            for (u32 i = k + 1; i < n; ++i) {
                auto row = a.p + usize(i) * n;
                u32 scale = M::mul(row[k], inverse);
                if (scale)
                    for (u32 j = k + 1; j < n; ++j)
                        row[j] = M::sub(row[j], M::mul(scale, pivot[j]));
            }
        }
    }
    return answer;
}
} // namespace toy
