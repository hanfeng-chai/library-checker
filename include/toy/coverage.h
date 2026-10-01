#pragma once
#include <toy/buffer.h>
namespace toy {
// One cache line stores eight cover counts and their child uncovered lengths.
struct CoverageTree {
    struct alignas(64) Node { u32 count[8]{}, gap[8]{}; };
    Buffer<Node> tree;
    std::array<u32, 12> offset{};
    u32 height = 0, total = 0, uncovered = 0;
    explicit CoverageTree(std::span<const u32> coordinates) {
        usize n = coordinates.empty() ? 0 : coordinates.size() - 1, groups = std::max<usize>(1, (n + 7) / 8), size = 0;
        for (;;) { offset[height++] = size; size += groups; if (groups == 1) break; groups = (groups + 7) / 8; }
        tree = Buffer<Node>(size); std::fill(tree.p, tree.p + size, Node{});
        for (usize i = 0; i < n; ++i) total += tree[i / 8].gap[i & 7] = coordinates[i + 1] - coordinates[i];
        for (u32 k = 1; k < height; ++k)
            for (u32 i = 0; i < offset[k] - offset[k - 1]; ++i) {
                u32 sum = 0; for (u32 x : tree[offset[k - 1] + i].gap) sum += x;
                tree[offset[k] + i / 8].gap[i & 7] = sum;
            }
        uncovered = total;
    }
    static u32 propagate(Node& node, u32 lane, u32 delta) {
        if (!delta) return 0;
        node.gap[lane] += delta; return node.count[lane] ? 0 : delta;
    }
    template<bool Insert> static u32 change(Node& node, u32 l, u32 r) {
        auto lanes = _mm256_setr_epi32(0,1,2,3,4,5,6,7);
        auto selected = _mm256_andnot_si256(_mm256_cmpgt_epi32(_mm256_set1_epi32(l), lanes), _mm256_cmpgt_epi32(_mm256_set1_epi32(r), lanes));
        auto old = _mm256_load_si256((const __m256i*)node.count);
        auto now = Insert ? _mm256_sub_epi32(old, selected) : _mm256_add_epi32(old, selected);
        _mm256_store_si256((__m256i*)node.count, now);
        auto crossing = _mm256_and_si256(selected, _mm256_cmpeq_epi32(Insert ? old : now, _mm256_setzero_si256()));
        if (_mm256_testz_si256(crossing, crossing)) return 0;
        auto gaps = _mm256_and_si256(crossing, _mm256_load_si256((const __m256i*)node.gap));
        auto sum = _mm_add_epi32(_mm256_castsi256_si128(gaps), _mm256_extracti128_si256(gaps, 1));
        sum = _mm_add_epi32(sum, _mm_srli_si128(sum, 8)); sum = _mm_add_epi32(sum, _mm_srli_si128(sum, 4));
        u32 delta = _mm_cvtsi128_si32(sum); return Insert ? -delta : delta;
    }
    template<bool Insert> void update(u32 l, u32 r) {
        if (l == r) return;
        u32 a = l, b = r - 1, da = 0, db = 0;
        for (u32 k = 0; k < height; ++k) {
            u32 x = a / 8, y = b / 8; auto& left = tree[offset[k] + x];
            da = propagate(left, a & 7, da);
            if (x == y) {
                da += propagate(left, b & 7, db); db = 0;
                if (l < r) da += change<Insert>(left, l - 8 * x, r - 8 * x);
                l = r = 0;
            } else {
                auto& right = tree[offset[k] + y]; db = propagate(right, b & 7, db);
                if (l < r) {
                    u32 first = l - 8 * x, last = r - 8 * y;
                    if (first && first != 8) da += change<Insert>(left, first, 8);
                    if (last && last != 8) db += change<Insert>(right, 0, last);
                    l = (l + 7) / 8; r /= 8;
                }
            }
            if (l >= r && !da && !db) return;
            a = x; b = y;
        }
        uncovered += da + db;
    }
    void add(u32 l, u32 r, int delta) { if (delta > 0) update<true>(l, r); else update<false>(l, r); }
    u32 covered() const { return total - uncovered; }
};
}
