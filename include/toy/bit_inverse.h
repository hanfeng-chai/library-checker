#pragma once
#include <toy/bit_matrix.h>
namespace toy {
// After full-rank forward elimination of [A|B], solve only B backwards.
// Earlier pivot columns are zero in these source rows, including boundary bits.
template <u32 Block = 8>
void bit_back_substitute(BitMatrix &a, u32 columns) {
    u32 first = columns / 64;
    Buffer<u64> table(usize(1u << Block) * a.stride);
    for (u32 end = a.n; end;) {
        u32 size = end % Block ? end % Block : Block, start = end - size;
        u32 lo = a.pivot[start] / 64, hi = a.pivot[end - 1] / 64;
        u64 mask0 = 0, mask1 = 0;
        for (u32 i = start; i < end; ++i) {
            u32 p = a.pivot[i];
            if (p / 64 == lo)
                mask0 |= 1ull << (p % 64);
            else if (p / 64 == lo + 1)
                mask1 |= 1ull << (p % 64);
        }
        std::fill(table.p + first, table.p + a.stride, 0ull);
        for (u32 key = 1; key < (1u << size); ++key) {
            u32 bit = std::countr_zero(key);
            u64 *dst = table.p + usize(key) * a.stride;
            const u64 *src = table.p + usize(key & (key - 1)) * a.stride, *row = a[start + bit];
            u32 j = first;
            for (; j + 4 <= a.stride; j += 4)
                _mm256_storeu_si256(
                    (__m256i *)(dst + j),
                    _mm256_xor_si256(_mm256_loadu_si256((const __m256i *)(src + j)),
                                     _mm256_loadu_si256((const __m256i *)(row + j))));
            for (; j < a.stride; ++j) dst[j] = src[j] ^ row[j];
        }
        for (u32 i = 0; i < start; ++i) {
            u32 key = 0;
            if (hi <= lo + 1) {
                key = _pext_u64(a[i][lo], mask0);
                if (mask1) key |= _pext_u64(a[i][lo + 1], mask1) << std::popcount(mask0);
            } else
                for (u32 j = 0; j < size; ++j) key |= u32(a.get(i, a.pivot[start + j])) << j;
            if (key) xor_words(a[i], table.p + usize(key) * a.stride, first, a.stride);
        }
        end = start;
    }
}
} // namespace toy
