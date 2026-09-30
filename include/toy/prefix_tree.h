#pragma once
#include <toy/buffer.h>

namespace toy {
// Nonnegative u32 weights with total <2^31; exclusive prefixes in 32-way nodes.
struct PrefixTree32 {
    Buffer<u32> tree;
    array<usize, 8> offset{};
    usize height = 0;
    u32 total = 0;
    inline static constexpr auto masks = [] {
        array<array<u32, 32>, 32> a{};
        for (int i = 0; i < 32; ++i) for (int j = i + 1; j < 32; ++j) a[i][j] = ~u32(0);
        return a;
    }();
    PrefixTree32() = default;
    void allocate(usize n) {
        usize size = 0; ++n;
        do { n = (n + 31) / 32; offset[height++] = size; size += 32 * n; } while (n > 1);
        tree = Buffer<u32>(size);
    }
    explicit PrefixTree32(usize n) { allocate(n); fill(tree.p, tree.p + tree.n, 0u); }
    explicit PrefixTree32(Buffer<u32> counts) {
        allocate(counts.n); counts.resize(counts.n + 1);
        for (usize k = 0; k < height; ++k) {
            usize n = (counts.n + 31) / 32; Buffer<u32> next(n);
            for (usize i = 0; i < n; ++i) {
                u32 sum = 0;
                for (usize j = 0; j < 32; ++j) {
                    tree[offset[k] + 32 * i + j] = sum;
                    if (32 * i + j < counts.n) sum += counts[32 * i + j];
                }
                next[i] = sum;
            }
            counts = std::move(next);
        }
        total = counts[0];
    }
    void add(usize i, i32 delta) {
        total += delta; auto change = _mm256_set1_epi32(delta);
        for (usize k = 0; k < height; ++k, i >>= 5) {
            auto* p = (__m256i*)(tree.p + offset[k] + (i & -usize(32)));
            const auto* m = (const __m256i*)masks[i & 31].data();
            for (int j = 0; j < 4; ++j)
                _mm256_store_si256(p + j, _mm256_add_epi32(_mm256_load_si256(p + j), _mm256_and_si256(change, _mm256_loadu_si256(m + j))));
        }
    }
    u32 prefix(usize end) const {
        u32 sum = 0; for (usize k = 0; k < height; ++k, end >>= 5) sum += tree[offset[k] + end]; return sum;
    }
    pair<u32,u32> select(u32 rank) const {
        usize block = 0;
        for (usize k = height; k--;) {
            const u32* p = tree.p + offset[k] + 32 * block;
            auto target = _mm256_set1_epi32(rank + 1); u32 mask = 0;
            for (int j = 0; j < 4; ++j) {
                auto before = _mm256_cmpgt_epi32(target, _mm256_load_si256((const __m256i*)(p + 8 * j)));
                mask |= u32(_mm256_movemask_ps(_mm256_castsi256_ps(before))) << (8 * j);
            }
            u32 child = popcount(mask) - 1; rank -= p[child]; block = 32 * block + child;
        }
        return {u32(block), rank};
    }
};
}
