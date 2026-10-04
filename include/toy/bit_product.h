#pragma once
#include <toy/bit_matrix.h>
namespace toy {
// Four 4-bit tables cover sixteen inner columns. A 256-row output tile and
// the tables fit L1; packed offsets avoid decoding four keys for every tile.
inline BitMatrix bit_product(const BitMatrix &a, const BitMatrix &b) {
    constexpr u32 Rows = 256;
    BitMatrix c(a.n, b.m);
    u32 groups = (a.m + 15) / 16;
    Buffer<u64> keys(usize(Rows) * groups);
    alignas(32) __m256i table[4][16], sum[Rows];
    for (u32 begin = 0; begin < a.n; begin += Rows) {
        u32 rows = std::min(Rows, a.n - begin);
        for (u32 g = 0; g < groups; ++g)
            for (u32 i = 0; i < rows; ++i) {
                u32 k = 16 * g;
                u16 bits = a[begin + i][k / 64] >> (k % 64);
                keys[usize(g) * rows + i] = _pdep_u64(bits, 0x000f000f000f000full) << 5;
            }
        for (u32 first = 0; first < b.stride; first += 4) {
            u32 tail = std::min(4u, b.stride - first);
            auto mask =
                _mm256_set_epi64x(tail > 3 ? -1 : 0, tail > 2 ? -1 : 0, tail > 1 ? -1 : 0, -1);
            for (u32 i = 0; i < rows; ++i) sum[i] = _mm256_setzero_si256();
            for (u32 g = 0; g < groups; ++g) {
                for (u32 part = 0; part < 4; ++part) {
                    table[part][0] = _mm256_setzero_si256();
                    for (u32 bit = 0; bit < 4; ++bit) {
                        u32 source = 16 * g + 4 * part + bit;
                        auto value = source < b.n
                                         ? _mm256_maskload_epi64(
                                               (const long long *)(b[source] + first), mask)
                                         : _mm256_setzero_si256();
                        for (u32 k = 0; k < (1u << bit); ++k)
                            table[part][k + (1u << bit)] = _mm256_xor_si256(table[part][k], value);
                    }
                }
                for (u32 i = 0; i < rows; ++i) {
                    u64 key = keys[usize(g) * rows + i];
                    auto x = sum[i];
                    x = _mm256_xor_si256(
                        x, _mm256_load_si256((const __m256i *)((const char *)table[0] + u16(key))));
                    x = _mm256_xor_si256(
                        x, _mm256_load_si256(
                               (const __m256i *)((const char *)table[1] + u16(key >> 16))));
                    x = _mm256_xor_si256(
                        x, _mm256_load_si256(
                               (const __m256i *)((const char *)table[2] + u16(key >> 32))));
                    x = _mm256_xor_si256(
                        x, _mm256_load_si256(
                               (const __m256i *)((const char *)table[3] + u16(key >> 48))));
                    sum[i] = x;
                }
            }
            for (u32 i = 0; i < rows; ++i)
                _mm256_maskstore_epi64((long long *)(c[begin + i] + first), mask, sum[i]);
        }
    }
    return c;
}
} // namespace toy
