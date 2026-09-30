#pragma once
#include <toy/buffer.h>

namespace toy {
template<class T> struct Fenwick {
    Buffer<T> tree;
    explicit Fenwick(usize n) : tree(n + 1) { fill(tree.p, tree.p + tree.n, T{}); }
    explicit Fenwick(span<const T> a) : Fenwick(a.size()) {
        for (usize i = 1; i < tree.n; ++i) {
            tree[i] += a[i - 1]; usize next = i + (i & -i);
            if (next < tree.n) tree[next] += tree[i];
        }
    }
    void add(usize i, T value) { for (++i; i < tree.n; i += i & -i) tree[i] += value; }
    T prefix(usize r) const { T sum{}; for (; r; r &= r - 1) sum += tree[r]; return sum; }
    T sum(usize l, usize r) const { return prefix(r) - prefix(l); }
};

// Each 16-way node stores exclusive child prefixes. One read per level answers
// a prefix; AVX2 adds a delta to all child prefixes after the changed position.
struct WideFenwick {
    Buffer<u64> tree;
    array<usize, 16> offset{};
    usize levels = 0;
    inline static constexpr auto masks = [] {
        array<array<u64, 16>, 16> a{};
        for (int i = 0; i < 16; ++i) for (int j = i + 1; j < 16; ++j) a[i][j] = ~u64(0);
        return a;
    }();
    explicit WideFenwick(Buffer<u64> a) {
        a.resize(a.n + 1); // Sentinel makes prefix(original_size) a normal query.
        usize count = a.n, total = 0;
        do { count = (count + 15) / 16; offset[levels++] = total; total += 16 * count; } while (count > 1);
        tree = Buffer<u64>(total);
        for (usize level = 0; level < levels; ++level) {
            usize count = (a.n + 15) / 16; Buffer<u64> next(count);
            for (usize i = 0; i < count; ++i) {
                u64 sum = 0;
                for (usize j = 0; j < 16; ++j) {
                    tree[offset[level] + 16 * i + j] = sum;
                    if (16 * i + j < a.n) sum += a[16 * i + j];
                }
                next[i] = sum;
            }
            a = std::move(next);
        }
    }
    void add(usize i, u64 value) {
        auto delta = _mm256_set1_epi64x(value);
        for (usize k = 0; k < levels; ++k, i >>= 4) {
            auto* p = (__m256i*)(tree.p + offset[k] + (i & -usize(16)));
            const auto* m = (const __m256i*)masks[i & 15].data();
            for (usize j = 0; j < 4; ++j)
                _mm256_store_si256(p + j, _mm256_add_epi64(_mm256_load_si256(p + j), _mm256_and_si256(delta, _mm256_loadu_si256(m + j))));
        }
    }
    u64 prefix(usize r) const {
        u64 sum = 0; for (usize k = 0; k < levels; ++k, r >>= 4) sum += tree[offset[k] + r]; return sum;
    }
    u64 sum(usize l, usize r) const { return prefix(r) - prefix(l); }
};
}
