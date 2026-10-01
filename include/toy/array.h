#pragma once
#include <toy/common.h>

namespace toy {
// Fixed-size owned array of constructed objects, including move-only Buffers.
template<class T>
struct Array {
    static_assert(alignof(T) <= 64);
    T* p;
    usize n;
    explicit Array(usize n = 0) : p(n ? (T*)aligned_alloc(64, (n * sizeof(T) + 63) & -usize(64)) : nullptr), n(n) {
        for (usize i = 0; i < n; ++i) std::construct_at(p + i);
    }
    Array(const Array&) = delete;
    Array(Array&& other) noexcept : p(std::exchange(other.p, nullptr)), n(std::exchange(other.n, 0)) {}
    ~Array() { for (usize i = 0; i < n; ++i) std::destroy_at(p + i); free(p); }
    T& operator[](usize i) { return p[i]; }
    const T& operator[](usize i) const { return p[i]; }
    operator std::span<T>() { return {p, n}; }
    operator std::span<const T>() const { return {p, n}; }
};
}
