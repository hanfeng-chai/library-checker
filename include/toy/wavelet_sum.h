#pragma once
#include <toy/radix_sort.h>
namespace toy {
template <class T = u64, bool Dual = false>
struct WaveletSum {
    struct Bits {
        u64 word;
        u32 before, padding;
    };
    Buffer<u32> xs, ys, position, zeros;
    Buffer<Bits> bits;
    Buffer<T> sums;
    u32 n = 0, height = 0, bit_stride = 0, stride = 0;
    std::array<usize, 33> offset{};
    void build_row(u32 level, std::span<const T> weights) {
        u32 count = level ? zeros[level - 1] : n;
        T *p = sums.p + offset[level];
        p[0] = {};
        for (u32 i = 0; i < count; ++i) p[i + 1] = weights[i];
        for (u32 i = 1; i <= count; ++i) {
            u32 next = i + (i & -i);
            if (next <= count) p[next] += p[i];
        }
    }
    WaveletSum(std::span<const u32> x, std::span<const u32> y, std::span<const T> weights)
        : xs(x.size()), ys(y.size()), position(x.size()), n(x.size()) {
        if (!n) return;
        struct Item {
            u32 value, id;
        };
        Buffer<Item> ordered(n);
        Buffer<u32> codes(n), current(n), next(n);
        Buffer<T> values(n), next_values(n);
        for (u32 i = 0; i < n; ++i) ordered[i] = {y[i], i};
        radix_sort(std::span(ordered.p, ordered.n), [](Item x) { return x.value; });
        ys.n = 0;
        for (u32 i = 0; i < n; ++i) {
            auto v = ordered[i];
            if (!i || v.value != ordered[i - 1].value) ys[ys.n++] = v.value;
            codes[v.id] = ys.n - 1;
        }
        for (u32 i = 0; i < n; ++i) ordered[i] = {x[i], i};
        radix_sort(std::span(ordered.p, ordered.n), [](Item x) { return x.value; });
        for (u32 i = 0; i < n; ++i) {
            u32 id = ordered[i].id;
            xs[i] = ordered[i].value;
            position[id] = i;
            current[i] = codes[id];
            values[i] = weights[id];
        }
        height = Dual ? std::bit_width(u32(ys.n - 1)) : std::bit_width(u32(ys.n));
        bit_stride = (n + 63) / 64 + 1;
        stride = n + 1;
        zeros = Buffer<u32>(height);
        bits = Buffer<Bits>(usize(height) * bit_stride);
        sums = Buffer<T>(usize(height + 1) * stride);
        if constexpr (Dual) {
            for (u32 i = 0; i < n; ++i) next_values[i] = values[i] - (i ? values[i - 1] : T{});
            build_row(0, std::span<const T>(next_values));
        }
        for (u32 level = 0; level < height; ++level) {
            u32 shift = height - level - 1, before = 0;
            auto *row = bits.p + usize(level) * bit_stride;
            for (u32 i = 0; i < n; i += 64) {
                u64 word = 0;
                for (u32 j = 0; j < std::min(64u, n - i); ++j)
                    word |= u64((current[i + j] >> shift) & 1) << j;
                row[i / 64] = {word, before, 0};
                before += std::popcount(word);
            }
            row[bit_stride - 1] = {0, before, 0};
            zeros[level] = n - before;
            offset[level + 1] = level ? offset[level] + zeros[level - 1] + 1 : (Dual ? n + 1 : 0);
            u32 zero = 0, one = zeros[level];
            for (u32 i = 0; i < n; ++i) {
                u32 at = current[i] >> shift & 1 ? one++ : zero++;
                next[at] = current[i];
                if constexpr (!Dual) next_values[at] = values[i];
            }
            std::swap(current, next);
            if constexpr (Dual)
                std::fill(sums.p + offset[level + 1], sums.p + offset[level + 1] + zeros[level] + 1,
                          T{});
            else {
                std::swap(values, next_values);
                build_row(level + 1, std::span<const T>(values));
            }
        }
    }
    u32 rank(u32 level, u32 end) const {
        auto x = bits[usize(level) * bit_stride + end / 64];
        return x.before + std::popcount(x.word & ((u64(1) << (end & 63)) - 1));
    }
    T range(u32 level, u32 l, u32 r) const {
        if (l == r) return {};
        const T *p = sums.p + offset[level];
        u32 common = l & (~0u << std::bit_width(l ^ r));
        T left{}, right{};
        for (; l != common; l &= l - 1) left += p[l];
        for (; r != common; r &= r - 1) right += p[r];
        return right - left;
    }
    void add_row(u32 level, u32 at, T value) {
        u32 count = level ? zeros[level - 1] : n;
        T *p = sums.p + offset[level];
        for (++at; at <= count; at += at & -at) p[at] += value;
    }
    void add(u32 id, T value)
        requires(!Dual)
    {
        u32 at = position[id];
        for (u32 level = 0; level < height; ++level) {
            u32 ones = rank(level, at);
            bool one = bits[usize(level) * bit_stride + at / 64].word >> (at & 63) & 1;
            if (one)
                at = zeros[level] + ones;
            else {
                at -= ones;
                add_row(level + 1, at, value);
            }
        }
    }
    T below_from(u32 level, u32 l, u32 r, u32 value) const {
        if (l == r || !value) return {};
        T result{};
        u32 stop = height - std::min(height, u32(std::countr_zero(value)));
        for (; level < stop; ++level) {
            u32 a = rank(level, l), b = rank(level, r);
            if (value >> (height - level - 1) & 1) {
                result += range(level + 1, l - a, r - b);
                l = zeros[level] + a;
                r = zeros[level] + b;
            } else {
                l -= a;
                r -= b;
            }
        }
        return result;
    }
    static std::pair<u32, u32> lower_pair(const u32 *p, u32 n, u32 a, u32 b) {
        if (!n) return {};
        u32 first = 0, last = 0;
        // Advance both independent searches together to overlap their memory loads.
        while (n > 1) {
            u32 half = n / 2, l = first + half, r = last + half;
            first = p[l] < a ? l : first;
            last = p[r] < b ? r : last;
            n -= half;
        }
        return {first + (p[first] < a), last + (p[last] < b)};
    }
    T rectangle(u32 left, u32 down, u32 right, u32 up) const
        requires(!Dual)
    {
        if (!n || left == right || down == up) return {};
        auto [l, r] = lower_pair(xs.p, n, left, right);
        auto [lo, hi] = lower_pair(ys.p, ys.n, down, up);
        if (l == r || lo == hi) return {};
        for (u32 level = 0; level < height; ++level) {
            u32 a = rank(level, l), b = rank(level, r), shift = height - level - 1, zl = l - a,
                zr = r - b, ol = zeros[level] + a, orr = zeros[level] + b;
            if (((lo ^ hi) >> shift) & 1)
                return range(level + 1, zl, zr) - below_from(level + 1, zl, zr, lo) +
                       below_from(level + 1, ol, orr, hi);
            if (lo >> shift & 1) {
                l = ol;
                r = orr;
            } else {
                l = zl;
                r = zr;
            }
        }
        return {};
    }
    void add_range(u32 level, u32 l, u32 r, T value)
        requires(Dual)
    {
        add_row(level, l, value);
        add_row(level, r, -value);
    }
    void add_below(u32 l, u32 r, u32 value, T delta)
        requires(Dual)
    {
        if (l == r || !value) return;
        if (value >= ys.n) {
            add_range(0, l, r, delta);
            return;
        }
        for (u32 level = 0; level < height; ++level) {
            u32 a = rank(level, l), b = rank(level, r);
            if (value >> (height - level - 1) & 1) {
                add_range(level + 1, l - a, r - b, delta);
                l = zeros[level] + a;
                r = zeros[level] + b;
            } else {
                l -= a;
                r -= b;
            }
        }
    }
    void add_rectangle(u32 left, u32 down, u32 right, u32 up, T value)
        requires(Dual)
    {
        if (!n || left == right || down == up) return;
        auto [l, r] = lower_pair(xs.p, n, left, right);
        auto [lo, hi] = lower_pair(ys.p, ys.n, down, up);
        add_below(l, r, hi, value);
        add_below(l, r, lo, -value);
    }
    T get(u32 id) const
        requires(Dual)
    {
        u32 at = position[id];
        T result = range(0, 0, at + 1);
        for (u32 level = 0; level < height; ++level) {
            u32 ones = rank(level, at);
            bool one = bits[usize(level) * bit_stride + at / 64].word >> (at & 63) & 1;
            if (one)
                at = zeros[level] + ones;
            else {
                at -= ones;
                result += range(level + 1, 0, at + 1);
            }
        }
        return result;
    }
};
} // namespace toy
