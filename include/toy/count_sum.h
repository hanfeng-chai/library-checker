#pragma once
#include <toy/fenwick.h>

namespace toy {
struct BoundedSum { u64 sum = 0; u32 count = 0; };
inline BoundedSum& operator+=(BoundedSum& a, BoundedSum b) { a.sum += b.sum; a.count += b.count; return a; }
inline BoundedSum operator-(BoundedSum a, BoundedSum b) { return {a.sum - b.sum, a.count - b.count}; }

// Each position is inserted at most once. Exclusive local prefixes cover fewer
// than 2^16 positions, so count and bounded sum fit together in a u64.
template<u32 MaxValue = ~u32(0)> struct CountSumTree {
    static constexpr usize chunk = 1 << 16;
    static constexpr unsigned shift = max<unsigned>(1, bit_width(u64(MaxValue) * chunk));
    static_assert(shift + 16 <= 64);
    static constexpr u64 mask = (u64(1) << shift) - 1;
    Buffer<u64> tree;
    array<usize, 4> offset{};
    usize height = 0;
    Fenwick<BoundedSum> coarse;
    explicit CountSumTree(usize n) : coarse((n + chunk - 1) / chunk) {
        usize size = 0; ++n;
        do { n = (n + 15) / 16; offset[height++] = size; size += 16 * n; } while (height < 4);
        usize bytes = (size * 8 + (1 << 21) - 1) & -usize(1 << 21); tree.p = (u64*)aligned_alloc(1 << 21, bytes); tree.n = tree.capacity = size; madvise(tree.p, bytes, MADV_HUGEPAGE); fill(tree.p, tree.p + size, u64(0));
    }
    void prefetch(usize position) const { __builtin_prefetch(tree.p + (position & -usize(16)), 1, 3); __builtin_prefetch(tree.p + (position & -usize(16)) + 8, 1, 3); }
    void add(usize position, u32 value) {
        coarse.add(position >> 16, {value, 1});
        auto delta = _mm256_set1_epi64x((u64(1) << shift) + value);
        for (usize i = position, k = 0; k < 4; ++k, i >>= 4) {
            auto* p = (__m256i*)(tree.p + offset[k] + (i & -usize(16)));
            const auto* m = (const __m256i*)WideFenwick::masks[i & 15].data();
            for (int j = 0; j < 4; ++j)
                _mm256_store_si256(p + j, _mm256_add_epi64(_mm256_load_si256(p + j), _mm256_and_si256(delta, _mm256_loadu_si256(m + j))));
        }
    }
    BoundedSum prefix(usize end) const {
        u64 local = 0; for (usize i = end, k = 0; k < 4; ++k, i >>= 4) local += tree[offset[k] + i];
        auto result = coarse.prefix(end >> 16);
        result.sum += local & mask; result.count += local >> shift; return result;
    }
    BoundedSum sum(usize l, usize r) const { return prefix(r) - prefix(l); }
};
}
