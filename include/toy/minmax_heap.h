#pragma once
#include <toy/buffer.h>

namespace toy {
// Alternating levels order minima and maxima. Push/pop are O(log n), build O(n).
template<class T, class Compare = less<T>> struct MinMaxHeap {
    Buffer<T> data;
    [[no_unique_address]] Compare compare{};
    usize maximum = 0;
    static bool min_level(usize i) { return bit_width(i + 1) & 1; }
    template<bool Min> bool better(T a, T b) const { return Min ? compare(a, b) : compare(b, a); }
    void refresh() { maximum = data.n <= 2 ? data.n - 1 : compare(data[1], data[2]) ? 2 : 1; }
    template<bool Min> void up(usize i, T value) {
        while (i >= 3) {
            usize grandparent = (i - 3) / 4;
            if (!better<Min>(value, data[grandparent])) break;
            data[i] = data[grandparent]; i = grandparent;
        }
        data[i] = value;
    }
    template<bool Min> void down(usize i) {
        T value = data[i];
        for (;;) {
            usize child = 2 * i + 1, grandchild = 4 * i + 3;
            if (child >= data.n) break;
            if (grandchild + 3 < data.n) {
                usize a = grandchild + better<Min>(data[grandchild + 1], data[grandchild]);
                usize b = grandchild + 2 + better<Min>(data[grandchild + 3], data[grandchild + 2]);
                usize best = better<Min>(data[b], data[a]) ? b : a;
                if (!better<Min>(data[best], value)) break;
                data[i] = data[best]; usize parent = (best - 1) / 2;
                // The moved value must also respect the opposite-order parent.
                if (better<Min>(data[parent], value)) swap(data[parent], value);
                i = best; continue;
            }
            usize best = child;
            if (child + 1 < data.n && better<Min>(data[child + 1], data[best])) ++best;
            for (usize j = grandchild; j < std::min(data.n, grandchild + 4); ++j)
                if (better<Min>(data[j], data[best])) best = j;
            if (better<Min>(data[best], value)) {
                data[i] = data[best];
                if (best >= grandchild) {
                    usize parent = (best - 1) / 2;
                    if (better<Min>(data[parent], value)) swap(data[parent], value);
                }
                data[best] = value; return;
            }
            break;
        }
        data[i] = value;
    }
    explicit MinMaxHeap(Buffer<T> values = {}, usize capacity = 0) : data(std::move(values)) {
        data.reserve(capacity);
        for (usize i = data.n / 2; i--;) { if (min_level(i)) down<true>(i); else down<false>(i); }
        if (data.n) refresh();
    }
    usize size() const { return data.n; }
    T min() const { return data[0]; }
    T max() const { return data[maximum]; }
    void push(T value) {
        if (data.n == data.capacity) data.reserve(std::max<usize>(4, 2 * data.n));
        usize i = data.n++; if (!i) { data[0] = value; maximum = 0; return; }
        usize parent = (i - 1) / 2;
        if (min_level(i)) {
            if (compare(data[parent], value)) { data[i] = data[parent]; up<false>(parent, value); }
            else up<true>(i, value);
        } else {
            if (compare(value, data[parent])) { data[i] = data[parent]; up<true>(parent, value); }
            else up<false>(i, value);
        }
        refresh();
    }
    template<bool Min> T pop() {
        usize i = Min ? 0 : maximum; T result = data[i]; data[i] = data[--data.n];
        if (i < data.n) down<Min>(i);
        if (data.n) refresh(); else maximum = 0;
        return result;
    }
    T pop_min() { return pop<true>(); }
    T pop_max() { return pop<false>(); }
};
}
