#pragma once
#include <toy/buffer.h>
namespace toy {
template <class T, class Operation>
struct BidirectionalTree {
    struct Node {
        T forward, reverse;
    };
    u32 capacity;
    T identity;
    Operation operation;
    Buffer<Node> tree;
    BidirectionalTree(std::span<const T> values, T unit, Operation op = {})
        : capacity(std::bit_ceil(std::max<usize>(1, values.size()))), identity(unit), operation(op),
          tree(2 * capacity) {
        std::fill(tree.p, tree.p + tree.n, Node{unit, unit});
        for (u32 i = 0; i < values.size(); ++i) tree[capacity + i] = {values[i], values[i]};
        for (u32 i = capacity; --i;) pull(i);
    }
    void pull(u32 i) {
        const auto &a = tree[2 * i];
        const auto &b = tree[2 * i + 1];
        tree[i] = {operation(a.forward, b.forward), operation(b.reverse, a.reverse)};
    }
    void set(u32 i, T value) {
        tree[i += capacity] = {value, value};
        while (i >>= 1) pull(i);
    }
    template <bool Reverse = false>
    T fold(u32 l, u32 r) const {
        T a = identity, b = identity;
        for (l += capacity, r += capacity; l < r; l >>= 1, r >>= 1) {
            if (l & 1) {
                if constexpr (Reverse)
                    a = operation(tree[l].reverse, a);
                else
                    a = operation(a, tree[l].forward);
                ++l;
            }
            if (r & 1) {
                --r;
                if constexpr (Reverse)
                    b = operation(b, tree[r].reverse);
                else
                    b = operation(tree[r].forward, b);
            }
        }
        if constexpr (Reverse)
            return operation(b, a);
        else
            return operation(a, b);
    }
};
} // namespace toy
