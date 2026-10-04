#pragma once
#include <toy/buffer.h>
namespace toy {
// Child minima are relative to their parent's minimum: every block has min 0.
struct RangeAddMin {
    static constexpr i64 infinity = 1ll << 60;
    struct alignas(64) Node {
        i64 value[8];
    };
    Buffer<Node> tree;
    std::array<u32, 12> offset{};
    u32 height = 0, n;
    i64 root;
    static __m256i min4(__m256i a, __m256i b) {
        return _mm256_blendv_epi8(a, b, _mm256_cmpgt_epi64(a, b));
    }
    static i64 reduce(__m256i x) {
        x = min4(x, _mm256_permute2x128_si256(x, x, 1));
        x = min4(x, _mm256_permute4x64_epi64(x, 0xb1));
        return _mm256_extract_epi64(x, 0);
    }
    static i64 normalize(Node &node) {
        auto a = _mm256_load_si256((const __m256i *)node.value),
             b = _mm256_load_si256((const __m256i *)(node.value + 4));
        i64 delta = reduce(min4(a, b));
        auto d = _mm256_set1_epi64x(delta);
        _mm256_store_si256((__m256i *)node.value, _mm256_sub_epi64(a, d));
        _mm256_store_si256((__m256i *)(node.value + 4), _mm256_sub_epi64(b, d));
        return delta;
    }
    static __m256i mask(u32 l, u32 r, u32 base) {
        auto lane = _mm256_setr_epi64x(base, base + 1, base + 2, base + 3);
        return _mm256_andnot_si256(_mm256_cmpgt_epi64(_mm256_set1_epi64x(l), lane),
                                   _mm256_cmpgt_epi64(_mm256_set1_epi64x(r), lane));
    }
    static void change(Node &node, u32 l, u32 r, i64 delta) {
        auto d = _mm256_set1_epi64x(delta);
        for (u32 i = 0; i < 8; i += 4) {
            auto *p = (__m256i *)(node.value + i);
            _mm256_store_si256(
                p, _mm256_add_epi64(_mm256_load_si256(p), _mm256_and_si256(d, mask(l, r, i))));
        }
    }
    static i64 minimum(const Node &node, u32 l, u32 r) {
        auto inf = _mm256_set1_epi64x(infinity);
        auto a =
            _mm256_blendv_epi8(inf, _mm256_load_si256((const __m256i *)node.value), mask(l, r, 0));
        auto b = _mm256_blendv_epi8(inf, _mm256_load_si256((const __m256i *)(node.value + 4)),
                                    mask(l, r, 4));
        return reduce(min4(a, b));
    }
    explicit RangeAddMin(std::span<const i64> values) : n(values.size()) {
        u32 groups = std::max<usize>(1, (values.size() + 7) / 8), size = 0;
        for (;;) {
            offset[height++] = size;
            size += groups;
            if (groups == 1) break;
            groups = (groups + 7) / 8;
        }
        tree = Buffer<Node>(size);
        for (auto &node : std::span(tree.p, tree.n))
            std::fill(node.value, node.value + 8, infinity);
        for (u32 i = 0; i < values.size(); ++i) tree[i / 8].value[i & 7] = values[i];
        for (u32 k = 0; k < height; ++k) {
            u32 end = k + 1 < height ? offset[k + 1] : size;
            for (u32 i = 0; i < end - offset[k]; ++i) {
                i64 delta = normalize(tree[offset[k] + i]);
                if (k + 1 < height)
                    tree[offset[k + 1] + i / 8].value[i & 7] = delta;
                else
                    root = delta;
            }
        }
    }
    void prefetch(u32 l, u32 r) const {
        __builtin_prefetch(tree.p + l / 8, 0, 3);
        if (r) __builtin_prefetch(tree.p + (r - 1) / 8, 0, 3);
    }
    void add(u32 l, u32 r, i64 amount) {
        if (l == r) return;
        if (l == 0 && r == n) {
            root += amount;
            return;
        }
        u32 a = l, b = r - 1;
        i64 da = 0, db = 0;
        for (u32 k = 0; k < height; ++k) {
            u32 x = a / 8, y = b / 8;
            auto &left = tree[offset[k] + x];
            left.value[a & 7] += da;
            if (x == y) {
                left.value[b & 7] += db;
                if (l < r) change(left, l - 8 * x, r - 8 * x, amount);
                da = normalize(left);
                db = 0;
                l = r = 0;
            } else {
                auto &right = tree[offset[k] + y];
                right.value[b & 7] += db;
                if (l < r) {
                    if (l & 7) change(left, l & 7, 8, amount);
                    if (r & 7) change(right, 0, r & 7, amount);
                    l = (l + 7) / 8;
                    r /= 8;
                }
                da = normalize(left);
                db = normalize(right);
            }
            if (l >= r && !da && !db) return;
            a = x;
            b = y;
        }
        root += da + db;
    }
    i64 minimum(u32 l, u32 r) const {
        if (l == r) return infinity;
        if (l == 0 && r == n) return root;
        u32 a = l, b = r - 1;
        i64 first = infinity, last = infinity;
        for (u32 k = 0; k < height; ++k) {
            u32 x = a / 8, y = b / 8;
            const auto &left = tree[offset[k] + x];
            first += left.value[a & 7];
            if (x == y) {
                last += left.value[b & 7];
                first = std::min(first, last);
                if (l < r) first = std::min(first, minimum(left, l - 8 * x, r - 8 * x));
                for (++k; k < height; ++k, x /= 8) first += tree[offset[k] + x / 8].value[x & 7];
                return first + root;
            }
            const auto &right = tree[offset[k] + y];
            last += right.value[b & 7];
            if (l < r) {
                if (l & 7) first = std::min(first, minimum(left, l & 7, 8));
                if (r & 7) last = std::min(last, minimum(right, 0, r & 7));
                l = (l + 7) / 8;
                r /= 8;
            }
            a = x;
            b = y;
        }
        return std::min(first, last) + root;
    }
};
} // namespace toy
