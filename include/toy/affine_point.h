#pragma once
#include <toy/affine.h>
#include <toy/buffer.h>

namespace toy {
// Redundant Montgomery values stay in [0,2P); omit final reductions in hot loops.
template <u32 P = 998244353>
struct AffinePointTree {
    using M = Mod<P>;
    static constexpr u32 one = (u64(1) << 32) % P;
    Buffer<u32> a, b, saved_a, saved_b;
    std::array<usize, 16> offset{};
    usize height = 0, leaves, stride, count = 0;
    static u32 multiply(u32 x, u32 y) {
        u64 z = u64(x) * y;
        return (z + u64(u32(z) * M::inverse) * P) >> 32;
    }
    static __m256i multiply(__m256i x, __m256i y) {
        auto inv = _mm256_set1_epi32(M::inverse), mod = _mm256_set1_epi32(P);
        auto even = _mm256_mul_epu32(x, y),
             odd = _mm256_mul_epu32(_mm256_srli_epi64(x, 32), _mm256_srli_epi64(y, 32));
        even = _mm256_add_epi64(even, _mm256_mul_epu32(_mm256_mul_epu32(even, inv), mod));
        odd = _mm256_add_epi64(odd, _mm256_mul_epu32(_mm256_mul_epu32(odd, inv), mod));
        return _mm256_or_si256(_mm256_srli_epi64(even, 32), odd);
    }
    static __m256i add(__m256i x, __m256i y) {
        auto z = _mm256_add_epi32(x, y);
        return _mm256_min_epu32(z, _mm256_sub_epi32(z, _mm256_set1_epi32(2 * P)));
    }
    AffinePointTree(std::span<const u32> values, usize queries)
        : leaves((values.size() + 8) & -usize(8)), stride((queries + 7) & -usize(8)) {
        usize size = 0, n = values.size() + 1;
        for (;;) {
            offset[height++] = size;
            size += (n + 7) & -usize(8);
            if (n <= 8) break;
            n = (n + 7) / 8;
        }
        a = Buffer<u32>(size - leaves);
        b = Buffer<u32>(size);
        std::fill(a.p, a.p + a.n, one);
        std::fill(b.p, b.p + b.n, 0u);
        if (!values.empty()) memcpy(b.p, values.data(), values.size_bytes());
        for (usize i = 0; i < leaves; i += 8)
            _mm256_store_si256(
                (__m256i *)(b.p + i),
                multiply(_mm256_load_si256((const __m256i *)(b.p + i)), _mm256_set1_epi32(M::r2)));
        saved_a = Buffer<u32>((height - 1) * stride);
        saved_b = Buffer<u32>(height * stride);
    }
    void block(usize level, usize index, __m256i multiplier, __m256i addition, __m256i mask) {
        u32 *target_b = b.p + offset[level] + index;
        auto old_b = _mm256_load_si256((const __m256i *)target_b);
        auto new_b = add(multiply(old_b, multiplier), addition);
        if (level) {
            u32 *target_a = a.p + offset[level] + index - leaves;
            auto old_a = _mm256_load_si256((const __m256i *)target_a);
            _mm256_store_si256((__m256i *)target_a,
                               _mm256_blendv_epi8(old_a, multiply(old_a, multiplier), mask));
        }
        _mm256_store_si256((__m256i *)target_b, _mm256_blendv_epi8(old_b, new_b, mask));
    }
    void push(usize level, usize index) {
        usize at = offset[level] + index;
        u32 m = a[at - leaves], c = b[at];
        if (m == one && !c) return;
        a[at - leaves] = one;
        b[at] = 0;
        block(level - 1, 8 * index, _mm256_set1_epi32(m), _mm256_set1_epi32(c),
              _mm256_set1_epi32(-1));
    }
    void apply(u32 l, u32 r, Affine<P> f) {
        if (l == r) return;
        for (usize k = height; --k;) {
            u32 first = l >> (3 * k), last = r >> (3 * k);
            push(k, first);
            if (last != first) push(k, last);
        }
        auto multiplier = _mm256_set1_epi32(M::mont(f.a, M::r2)),
             addition = _mm256_set1_epi32(M::mont(f.b, M::r2));
        auto lanes = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        auto mask = [&](u32 first, u32 last) {
            return _mm256_andnot_si256(_mm256_cmpgt_epi32(_mm256_set1_epi32(first), lanes),
                                       _mm256_cmpgt_epi32(_mm256_set1_epi32(last), lanes));
        };
        for (usize k = 0; l < r; ++k) {
            if (l / 8 == r / 8) {
                block(k, l & -8u, multiplier, addition, mask(l & 7, r & 7));
                break;
            }
            if (l & 7) block(k, l & -8u, multiplier, addition, mask(l & 7, 8));
            if (r & 7) block(k, r & -8u, multiplier, addition, mask(0, r & 7));
            l = (l + 7) / 8;
            r /= 8;
        }
    }
    u32 get(u32 index) const {
        u32 value = b[index];
        for (usize k = 1; k < height; ++k) {
            usize at = offset[k] + (index >> (3 * k));
            value = multiply(value, a[at - leaves]) + b[at];
            value = std::min(value, value - 2 * P);
        }
        value = multiply(value, 1);
        return std::min(value, value - P);
    }
    void collect(u32 index) {
        saved_b[count] = b[index];
        for (usize k = 1; k < height; ++k) {
            usize at = offset[k] + (index >> (3 * k));
            saved_a[(k - 1) * stride + count] = a[at - leaves];
            saved_b[k * stride + count] = b[at];
        }
        ++count;
    }
    std::span<const u32> resolve() {
        usize padded = (count + 7) & -usize(8);
        for (usize i = count; i < padded; ++i) {
            saved_b[i] = 0;
            for (usize k = 1; k < height; ++k)
                saved_a[(k - 1) * stride + i] = one, saved_b[k * stride + i] = 0;
        }
        for (usize i = 0; i < count; i += 8) {
            auto value = _mm256_load_si256((const __m256i *)(saved_b.p + i));
            for (usize k = 1; k < height; ++k)
                value =
                    add(multiply(value, _mm256_load_si256(
                                            (const __m256i *)(saved_a.p + (k - 1) * stride + i))),
                        _mm256_load_si256((const __m256i *)(saved_b.p + k * stride + i)));
            value = multiply(value, _mm256_set1_epi32(1));
            _mm256_store_si256(
                (__m256i *)(saved_b.p + i),
                _mm256_min_epu32(value, _mm256_sub_epi32(value, _mm256_set1_epi32(P))));
        }
        auto result = std::span<const u32>(saved_b.p, count);
        count = 0;
        return result;
    }
};
} // namespace toy
