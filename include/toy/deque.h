#pragma once
#include <toy/buffer.h>

namespace toy {
// At most capacity pushes to either end during the object's lifetime.
template <class T>
struct Deque {
    Buffer<T> data;
    usize first, last;
    explicit Deque(usize capacity) : data(2 * capacity + 1), first(capacity), last(capacity) {}
    void push_front(T x) { data[--first] = x; }
    void push_back(T x) { data[last++] = x; }
    void pop_front() { ++first; }
    void pop_back() { --last; }
    usize size() const { return last - first; }
    T &operator[](usize i) { return data[first + i]; }
    const T &operator[](usize i) const { return data[first + i]; }
};
} // namespace toy
