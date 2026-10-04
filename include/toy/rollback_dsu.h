#pragma once
#include <toy/buffer.h>

namespace toy {
struct RollbackDSU {
    struct Change {
        int root, child, size;
    };
    Buffer<i32> parent;
    explicit RollbackDSU(usize n) : parent(n) { std::fill(parent.p, parent.p + n, -1); }
    int leader(int x) const {
        while (parent[x] >= 0) x = parent[x];
        return x;
    }
    bool same(int a, int b) const { return leader(a) == leader(b); }
    Change merge(int a, int b) {
        a = leader(a);
        b = leader(b);
        if (a == b) return {-1, 0, 0};
        if (parent[a] > parent[b]) std::swap(a, b);
        Change change{a, b, parent[b]};
        parent[a] += parent[b];
        parent[b] = a;
        return change;
    }
    void undo(Change change) {
        if (change.root < 0) return;
        parent[change.root] -= change.size;
        parent[change.child] = change.size;
    }
};
} // namespace toy
