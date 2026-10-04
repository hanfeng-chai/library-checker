#pragma once
#include <toy/radix_sort.h>
namespace toy {
// Semi-local LCS seaweed labels. x[j] is the entrance reaching bottom column j;
// negative entrances lie on the left boundary and represent matched symbols.
inline void lcs_row(char c, std::string_view t, std::span<i32> x) {
    i32 carry = -1;
    for (u32 j = 0; j < t.size(); ++j)
        if (c == t[j] || carry > x[j]) std::swap(carry, x[j]);
}
// Query record: (a << 39) | (b << 29) | (c << 19) | original index.
// |s|,|t| < 1024, query count < 2^19. Results remain in input order.
inline Buffer<u32> prefix_substring_lcs(std::string_view s, std::string_view t,
                                        Buffer<u64> queries) {
    u32 n = t.size(), q = queries.n;
    Buffer<u32> answer(q);
    radix_sort<20, 10>(std::span(queries.p, queries.n), [](u64 x) { return x >> 29; });
    Buffer<i32> x(n), inverse(n);
    std::iota(x.p, x.p + n, 0);
    alignas(32) u32 prefix[16];
    u64 bits[16];
    u32 at = 0;
    for (u32 a = 0; a <= s.size() && at < q; ++a) {
        if (a) lcs_row(s[a - 1], t, x);
        if ((queries[at] >> 39) != a) continue;
        std::fill(inverse.p, inverse.p + n, -1);
        std::fill(bits, bits + 16, 0ull);
        prefix[0] = 0;
        for (u32 j = 0; j < n; ++j)
            if (x[j] < 0)
                bits[j / 64] |= 1ull << (j % 64);
            else
                inverse[x[j]] = j;
        for (u32 j = 1; j < 16; ++j) prefix[j] = prefix[j - 1] + std::popcount(bits[j - 1]);
        u32 threshold = 0;
        while (at < q && (queries[at] >> 39) == a) {
            u64 record = queries[at++];
            u32 b = (record >> 29) & 1023, c = (record >> 19) & 1023,
                id = record & ((1u << 19) - 1);
            while (threshold < b) {
                i32 position = inverse[threshold++];
                if (position < 0) continue;
                u32 word = position / 64;
                bits[word] |= 1ull << (position % 64);
                // There are at most 16 bitmap words. Increment their prefix
                // ranks with two vector comparisons instead of a Fenwick walk.
                auto w = _mm256_set1_epi32(word);
                for (u32 j = 0; j < 16; j += 8) {
                    auto index =
                        _mm256_setr_epi32(j, j + 1, j + 2, j + 3, j + 4, j + 5, j + 6, j + 7);
                    auto value = _mm256_load_si256((const __m256i *)(prefix + j));
                    value = _mm256_sub_epi32(value, _mm256_cmpgt_epi32(index, w));
                    _mm256_store_si256((__m256i *)(prefix + j), value);
                }
            }
            answer[id] =
                prefix[c / 64] + std::popcount(bits[c / 64] & ((1ull << (c % 64)) - 1)) - b;
        }
    }
    return answer;
}
struct PrefixSubstringLCS {
    Buffer<u16> rows;
    u32 width;
    PrefixSubstringLCS(std::string_view s, std::string_view t)
        : rows((s.size() + 1) * ((t.size() + 15) & ~usize(15))),
          width((t.size() + 15) & ~usize(15)) {
        Buffer<i32> x(t.size());
        std::iota(x.p, x.p + x.n, 0);
        for (u32 a = 0; a <= s.size(); ++a) {
            if (a) lcs_row(s[a - 1], t, x);
            for (u32 j = 0; j < t.size(); ++j) rows[usize(a) * width + j] = x[j] + 1;
        }
    }
    u32 query(u32 a, u32 b, u32 c) const {
        const u16 *p = rows.p + usize(a) * width;
        u32 result = 0;
        auto bound = _mm256_set1_epi16(b + 1);
        for (; b + 16 <= c; b += 16) {
            auto value = _mm256_loadu_si256((const __m256i *)(p + b));
            result +=
                std::popcount(u32(_mm256_movemask_epi8(_mm256_cmpgt_epi16(bound, value)))) / 2;
        }
        for (; b < c; ++b) result += p[b] < _mm256_extract_epi16(bound, 0);
        return result;
    }
};
} // namespace toy
