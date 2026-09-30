#pragma once
#include <toy/common.h>

namespace toy {

// Uninitialized, 64-byte-aligned storage for trivial values. Growing zero-fills.
template<class T>
struct Buffer {
    static_assert(is_trivially_copyable_v<T> && alignof(T) <= 64);
    T* p;
    usize n, capacity;

    [[gnu::always_inline]] Buffer(usize size = 0, usize reserve = 0)
        : p(allocate(max(size, reserve))), n(size), capacity(max(size, reserve)) {}
    Buffer(const Buffer&) = delete;
    [[gnu::always_inline]] Buffer(Buffer&& other) noexcept
        : p(exchange(other.p, nullptr)), n(exchange(other.n, 0)), capacity(exchange(other.capacity, 0)) {}
    [[gnu::always_inline]] ~Buffer() { free(p); }
    [[gnu::always_inline]] Buffer& operator=(Buffer other) noexcept {
        swap(p, other.p); swap(n, other.n); swap(capacity, other.capacity);
        return *this;
    }
    [[gnu::always_inline]] static T* allocate(usize count) {
        return count ? (T*)aligned_alloc(64, (count * sizeof(T) + 63) & -usize(64)) : nullptr;
    }
    [[gnu::always_inline]] void reserve(usize count) {
        if (count <= capacity) return;
        T* next = allocate(count);
        if (n) memcpy(next, p, n * sizeof(T));
        free(p); p = next; capacity = count;
    }
    [[gnu::always_inline]] void resize(usize size) {
        reserve(size);
        if (size > n) fill(p + n, p + size, T{});
        n = size;
    }
    [[gnu::always_inline]] T& operator[](usize i) { return p[i]; }
    [[gnu::always_inline]] const T& operator[](usize i) const { return p[i]; }
    [[gnu::always_inline]] operator span<T>() { return {p, n}; }
    [[gnu::always_inline]] operator span<const T>() const { return {p, n}; }
};

}
