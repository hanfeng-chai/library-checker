#pragma once
#include <toy/affine.h>
#include <toy/buffer.h>
#include <toy/montgomery.h>

namespace toy {
template <u32 P = 998244353>
struct RangeSetComposite {
    template <class T>
    static Buffer<T> storage(usize n) {
        if (n * sizeof(T) < (1 << 20)) return Buffer<T>(n);
        usize bytes = (n * sizeof(T) + (1 << 21) - 1) & -usize(1 << 21);
        Buffer<T> result;
        result.p = (T *)aligned_alloc(1 << 21, bytes);
        result.n = result.capacity = n;
        madvise(result.p, bytes, MADV_HUGEPAGE);
        return result;
    }
    using Function = Affine<P>;
    using R = Montgomery<P>;
    static Function compose(Function f, Function g) {
        return {R::multiply(f.a, g.a), R::add(R::multiply(g.a, f.b), g.b)};
    }
    static Function encode(Function f) { return {R::encode(f.a), R::encode(f.b)}; }
    static Function decode(Function f) { return {R::decode(f.a), R::decode(f.b)}; }
    usize capacity, height;
    Buffer<Function> tree, powers;
    Buffer<i32> lazy;
    explicit RangeSetComposite(std::span<const Function> values, usize assignments = 0)
        : capacity(std::bit_ceil(std::max<usize>(1, values.size()))),
          height(std::countr_zero(capacity)), tree(storage<Function>(2 * capacity)),
          powers(storage<Function>(assignments * (height + 1))), lazy(storage<i32>(2 * capacity)) {
        powers.n = 0;
        std::fill(tree.p, tree.p + tree.n, Function{R::one, 0});
        std::fill(lazy.p, lazy.p + lazy.n, -1);
        for (usize i = 0; i < values.size(); ++i) tree[capacity + i] = encode(values[i]);
        for (usize i = capacity; --i;) pull(i);
    }
    void cover(usize i, usize level, i32 base) {
        tree[i] = powers[base + level];
        lazy[i] = base;
    }
    void push(usize i, usize level) {
        if (lazy[i] < 0) return;
        cover(2 * i, level - 1, lazy[i]);
        cover(2 * i + 1, level - 1, lazy[i]);
        lazy[i] = -1;
    }
    void pull(usize i) { tree[i] = compose(tree[2 * i], tree[2 * i + 1]); }
    void boundary(u32 l, u32 r) {
        for (usize k = height; k; --k) {
            if ((l >> k << k) != l) push(l >> k, k);
            if ((r >> k << k) != r) push((r - 1) >> k, k);
        }
    }
    void set(u32 l, u32 r, Function f) {
        if (l == r) return;
        usize levels = std::bit_width(r - l), base = powers.n;
        if (base + levels > powers.capacity)
            powers.reserve(std::max(base + levels, 2 * powers.capacity));
        powers[powers.n++] = encode(f);
        for (usize k = 1; k < levels; ++k) {
            auto previous = powers[powers.n - 1];
            powers[powers.n++] = compose(previous, previous);
        }
        u32 a = l + capacity, b = r + capacity;
        boundary(a, b);
        for (u32 x = a, y = b, level = 0; x < y; x >>= 1, y >>= 1, ++level) {
            if (x & 1) cover(x++, level, base);
            if (y & 1) cover(--y, level, base);
        }
        for (usize k = 1; k <= height; ++k) {
            if ((a >> k << k) != a) pull(a >> k);
            if ((b >> k << k) != b) pull((b - 1) >> k);
        }
    }
    Function repeat(i32 base, u32 count) const {
        Function f = powers[base + std::countr_zero(count)];
        for (count &= count - 1; count; count &= count - 1)
            f = compose(f, powers[base + std::countr_zero(count)]);
        return f;
    }
    Function prefix(u32 r, usize i, usize width) const {
        Function result{R::one, 0};
        while (r != width && lazy[i] < 0) {
            width /= 2;
            i *= 2;
            if (r > width) {
                result = compose(result, tree[i]);
                ++i;
                r -= width;
            }
        }
        return compose(result, lazy[i] < 0 ? tree[i] : repeat(lazy[i], r));
    }
    Function suffix(u32 l, usize i, usize width) const {
        Function result{R::one, 0};
        while (l && lazy[i] < 0) {
            width /= 2;
            i *= 2;
            if (l < width)
                result = compose(tree[i + 1], result);
            else {
                ++i;
                l -= width;
            }
        }
        return compose(lazy[i] < 0 ? tree[i] : repeat(lazy[i], width - l), result);
    }
    Function query(u32 l, u32 r, usize i, usize width) const {
        for (;;) {
            if (!l && r == width) return tree[i];
            if (lazy[i] >= 0) return repeat(lazy[i], r - l);
            usize half = width / 2;
            if (r <= half) {
                i *= 2;
                width = half;
            } else if (l >= half) {
                i = 2 * i + 1;
                l -= half;
                r -= half;
                width = half;
            } else
                return compose(suffix(l, 2 * i, half), prefix(r - half, 2 * i + 1, half));
        }
    }
    Function fold(u32 l, u32 r) const {
        return l == r ? Function{} : decode(query(l, r, 1, capacity));
    }
    u32 evaluate(u32 l, u32 r, u32 x) const {
        if (l == r) return x;
        auto f = query(l, r, 1, capacity);
        u32 result = R::add(R::multiply(f.a, x), R::multiply(f.b, 1));
        return std::min(result, result - P);
    }
};
} // namespace toy
