#pragma once
#include <toy/affine.h>
#include <toy/buffer.h>
#include <toy/montgomery.h>
namespace toy {
// Each leaf is a coordinate interval; values are constant within an interval.
template <u32 P = 998244353>
struct WeightedAffineSum {
    using R = Montgomery<P>;
    using Function = Affine<P>;
    struct Node {
        u32 sum = 0, length = 0;
        Function lazy{R::one, 0};
    };
    Buffer<Node> tree;
    Buffer<u32> coordinates;
    u32 capacity;
    static bool identity(Function f) { return f.a == R::one && !f.b; }
    static u32 mapped(u32 value, u32 length, Function f) {
        u64 z = u64(value) * f.a + u64(length) * f.b;
        u32 result = (z + u64(u32(z) * Mod<P>::inverse) * P) >> 32;
        return std::min(result, result - 2 * P);
    }
    explicit WeightedAffineSum(std::span<const u32> points, std::span<const u32> values = {})
        : coordinates(points.size()),
          capacity(std::bit_ceil(std::max<usize>(1, points.empty() ? 0 : points.size() - 1))) {
        if (!points.empty()) memcpy(coordinates.p, points.data(), points.size_bytes());
        tree = Buffer<Node>(2 * capacity);
        std::fill(tree.p, tree.p + tree.n, Node{});
        for (u32 i = 0; i + 1 < points.size(); ++i) {
            auto &x = tree[capacity + i];
            x.length = points[i + 1] - points[i];
            if (!values.empty()) x.sum = u64(values[i]) * x.length % P;
        }
        for (u32 i = capacity; --i;) {
            tree[i].length = tree[2 * i].length + tree[2 * i + 1].length;
            pull(i);
        }
    }
    void pull(u32 i) { tree[i].sum = R::add(tree[2 * i].sum, tree[2 * i + 1].sum); }
    void transform(u32 i, Function f) {
        auto &x = tree[i];
        x.sum = mapped(x.sum, x.length, f);
        x.lazy = {R::multiply(x.lazy.a, f.a), R::add(R::multiply(x.lazy.b, f.a), f.b)};
    }
    void push(u32 i) {
        auto f = tree[i].lazy;
        if (identity(f)) return;
        transform(2 * i, f);
        transform(2 * i + 1, f);
        tree[i].lazy = {R::one, 0};
    }
    void update(u32 i, u32 l, u32 r, u32 first, u32 last, Function f) {
        if (first <= l && r <= last) {
            transform(i, f);
            return;
        }
        push(i);
        u32 m = (l + r) / 2;
        if (first < m) update(2 * i, l, m, first, last, f);
        if (last > m) update(2 * i + 1, m, r, first, last, f);
        pull(i);
    }
    u32 query(u32 i, u32 l, u32 r, u32 first, u32 last) const {
        const auto &x = tree[i];
        if (first <= l && r <= last) return x.sum;
        u32 m = (l + r) / 2, value;
        if (last <= m)
            value = query(2 * i, l, m, first, last);
        else if (first >= m)
            value = query(2 * i + 1, m, r, first, last);
        else
            value = R::add(query(2 * i, l, m, first, last), query(2 * i + 1, m, r, first, last));
        return identity(x.lazy)
                   ? value
                   : mapped(value, coordinates[std::min(r, last)] - coordinates[std::max(l, first)],
                            x.lazy);
    }
    void apply(u32 l, u32 r, Function f) {
        if (l < r) update(1, 0, capacity, l, r, {R::encode(f.a), R::encode(f.b)});
    }
    u32 sum(u32 l, u32 r) const {
        if (l == r) return 0;
        u32 value = query(1, 0, capacity, l, r);
        return std::min(value, value - P);
    }
};
} // namespace toy
