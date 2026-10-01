#pragma once
#include <toy/buffer.h>

namespace toy {
struct DSU {
    Buffer<i32> parent;
    explicit DSU(usize n) : parent(n) { std::fill(parent.p, parent.p + n, -1); }
    int leader(int x) {
        while (parent[x] >= 0) {
            int y = parent[x];
            if (parent[y] < 0) return y;
            x = parent[x] = parent[y];
        }
        return x;
    }
    bool merge(int a, int b) {
        a = leader(a); b = leader(b);
        if (a == b) return false;
        if (parent[a] > parent[b]) std::swap(a, b);
        parent[a] += parent[b]; parent[b] = a; return true;
    }
    bool same(int a, int b) { return leader(a) == leader(b); }
    int size(int x) { return -parent[leader(x)]; }
};
}
