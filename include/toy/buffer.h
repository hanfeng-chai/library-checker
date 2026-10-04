#pragma once
#include <toy/common.h>

namespace toy {

// Uninitialized, 64-byte-aligned storage for trivial values. Growing zero-fills.
template <class T>
struct Buffer {
    static_assert(std::is_trivially_copyable_v<T> && alignof(T) <= 64);
    T *p;
    usize n, capacity;

    [[gnu::always_inline]] Buffer(usize size = 0, usize reserve = 0)
        : p(allocate(std::max(size, reserve))), n(size), capacity(std::max(size, reserve)) {}
    Buffer(const Buffer &) = delete;
    [[gnu::always_inline]] Buffer(Buffer &&other) noexcept
        : p(std::exchange(other.p, nullptr)), n(std::exchange(other.n, 0)),
          capacity(std::exchange(other.capacity, 0)) {}
    [[gnu::always_inline]] ~Buffer() { free(p); }
    [[gnu::always_inline]] Buffer &operator=(Buffer other) noexcept {
        std::swap(p, other.p);
        std::swap(n, other.n);
        std::swap(capacity, other.capacity);
        return *this;
    }
    [[gnu::always_inline]] static T *allocate(usize count) {
        return count ? (T *)aligned_alloc(64, (count * sizeof(T) + 63) & -usize(64)) : nullptr;
    }
    [[gnu::always_inline]] void reserve(usize count) {
        if (count <= capacity) return;
        T *next = allocate(count);
        if (n) memcpy(next, p, n * sizeof(T));
        free(p);
        p = next;
        capacity = count;
    }
    [[gnu::always_inline]] void resize(usize size) {
        reserve(size);
        if (size > n) std::fill(p + n, p + size, T{});
        n = size;
    }
    [[gnu::always_inline]] T &operator[](usize i) { return p[i]; }
    [[gnu::always_inline]] const T &operator[](usize i) const { return p[i]; }
    [[gnu::always_inline]] operator std::span<T>() { return {p, n}; }
    [[gnu::always_inline]] operator std::span<const T>() const { return {p, n}; }
};

} // namespace toy
