#pragma once
#include <toy/buffer.h>

namespace toy {
// Associative operation, explicit identity. Capacity bounds lifetime pushes.
// Raw back values become suffix aggregates when transferred to the front.
template<class T, class Op> struct FoldQueue {
    Buffer<T> data;
    T identity, back;
    Op operation;
    usize first = 0, middle = 0, last = 0;
    FoldQueue(usize capacity, T identity = {}, Op operation = {}) : data(capacity), identity(identity), back(identity), operation(operation) {}
    void push(T value) {
        back = middle == last ? value : operation(back, value); data[last++] = value;
    }
    void pop() {
        if (first == middle) {
            for (usize i = last - 1; i > first; --i) data[i - 1] = operation(data[i - 1], data[i]);
            middle = last; back = identity;
        }
        ++first;
    }
    usize size() const { return last - first; }
    T fold() const { return first == middle ? back : middle == last ? data[first] : operation(data[first], back); }
};

// One centred buffer; each half stores aggregates toward its own outer end.
// When a pop exhausts one half, split the remaining sequence evenly and rebuild.
template<class T, class Op> struct FoldDeque {
    struct Node { T value, aggregate; };
    Buffer<Node> data;
    T identity;
    Op operation;
    usize first, middle, last;
    FoldDeque(usize capacity, T identity = {}, Op operation = {}) : data(2 * capacity + 1), identity(identity), operation(operation), first(capacity), middle(capacity), last(capacity) {}
    void push_front(T value) { T aggregate = first == middle ? value : operation(value, data[first].aggregate); data[--first] = {value, aggregate}; }
    void push_back(T value) { T aggregate = last == middle ? value : operation(data[last - 1].aggregate, value); data[last++] = {value, aggregate}; }
    void rebalance(bool front) {
        middle = first + (last - first + front) / 2;
        if (first < middle) {
            data[middle - 1].aggregate = data[middle - 1].value;
            for (usize i = middle - 1; i > first; --i) data[i - 1].aggregate = operation(data[i - 1].value, data[i].aggregate);
        }
        if (middle < last) {
            data[middle].aggregate = data[middle].value;
            for (usize i = middle + 1; i < last; ++i) data[i].aggregate = operation(data[i - 1].aggregate, data[i].value);
        }
    }
    void pop_front() { if (first == middle) rebalance(true); ++first; }
    void pop_back() { if (last == middle) rebalance(false); --last; }
    usize size() const { return last - first; }
    T fold() const {
        if (first == middle) return middle == last ? identity : data[last - 1].aggregate;
        return middle == last ? data[first].aggregate : operation(data[first].aggregate, data[last - 1].aggregate);
    }
};
}
