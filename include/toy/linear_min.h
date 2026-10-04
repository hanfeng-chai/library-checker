#pragma once
#include <toy/buffer.h>
namespace toy {
struct LinearAddMin {
    static constexpr i64 infinity = 2000000000000000000ll;
    struct Node {
        i64 minimum = infinity, forward = infinity, backward = infinity, a = 0, b = 0;
        u32 index = 0;
    };
    Buffer<Node> tree;
    u32 capacity, height;
    void pull(u32 i) {
        auto &x = tree[i];
        const auto &l = tree[2 * i];
        const auto &r = tree[2 * i + 1];
        const auto &best = l.minimum <= r.minimum ? l : r;
        x.minimum = best.minimum;
        x.index = best.index;
        x.forward = std::min(l.forward, r.forward);
        x.backward = std::min(l.backward, r.backward);
        if (l.minimum == infinity || r.minimum == infinity) return;
        if (l.minimum <= r.minimum)
            x.backward = std::min(x.backward, (r.minimum - l.minimum) / (r.index - l.index));
        else
            x.forward = std::min(x.forward, (l.minimum - r.minimum) / (r.index - l.index));
    }
    explicit LinearAddMin(std::span<const i64> input)
        : tree(2 * std::bit_ceil(std::max<usize>(1, input.size()))), capacity(tree.n / 2),
          height(std::countr_zero(capacity)) {
        std::fill(tree.p, tree.p + tree.n, Node{});
        for (u32 i = 0; i < input.size(); ++i) {
            tree[capacity + i].minimum = input[i];
            tree[capacity + i].index = i;
        }
        for (u32 i = capacity; --i;) pull(i);
    }
    void shift(u32 i, i64 a, i64 b) {
        auto &x = tree[i];
        if (x.minimum == infinity) return;
        x.minimum += a * x.index + b;
        if (i < capacity) {
            x.a += a;
            x.b += b;
            x.forward = std::min(infinity, x.forward - a);
            x.backward = std::min(infinity, x.backward + a);
        }
    }
    void push(u32 i) {
        auto &x = tree[i];
        if (x.a || x.b) {
            shift(2 * i, x.a, x.b);
            shift(2 * i + 1, x.a, x.b);
            x.a = x.b = 0;
        }
    }
    void apply(u32 i, i64 a, i64 b) {
        auto &x = tree[i];
        if (i >= capacity || (a >= 0 ? a <= x.forward : -a <= x.backward)) {
            shift(i, a, b);
            return;
        }
        push(i);
        apply(2 * i, a, b);
        apply(2 * i + 1, a, b);
        pull(i);
    }
    void add(u32 l, u32 r, i64 a, i64 b) {
        if (l == r) return;
        u32 first = l + capacity, last = r + capacity;
        for (u32 k = height; k; --k) {
            if ((first >> k << k) != first) push(first >> k);
            if ((last >> k << k) != last) push((last - 1) >> k);
        }
        for (u32 x = first, y = last; x < y; x >>= 1, y >>= 1) {
            if (x & 1) apply(x++, a, b);
            if (y & 1) apply(--y, a, b);
        }
        for (u32 k = 1; k <= height; ++k) {
            if ((first >> k << k) != first) pull(first >> k);
            if ((last >> k << k) != last) pull((last - 1) >> k);
        }
    }
    i64 query(u32 i, u32 l, u32 r, u32 first, u32 last, i64 a, i64 b) const {
        const auto &x = tree[i];
        if (first <= l && r <= last) return x.minimum + a * x.index + b;
        a += x.a;
        b += x.b;
        u32 m = (l + r) / 2;
        if (last <= m) return query(2 * i, l, m, first, last, a, b);
        if (first >= m) return query(2 * i + 1, m, r, first, last, a, b);
        return std::min(query(2 * i, l, m, first, last, a, b),
                        query(2 * i + 1, m, r, first, last, a, b));
    }
    i64 minimum(u32 l, u32 r) const {
        return l == r ? infinity : query(1, 0, capacity, l, r, 0, 0);
    }
};
} // namespace toy
