#pragma once
#include <toy/buffer.h>
namespace toy {
struct WeightedTreeOrder {
    u64 cost;
    Buffer<u32> order;
};
inline WeightedTreeOrder minimum_tree_order(std::span<const u32> parent, std::span<const u32> c,
                                            std::span<const u32> d) {
    u32 n = parent.size();
    WeightedTreeOrder result{0, Buffer<u32>(n)};
    if (!n) return result;
    struct Block {
        u32 c, d, id;
    };
    auto better = [](Block a, Block b) {
        bool az = !(a.c | a.d), bz = !(b.c | b.d);
        if (az != bz) return !az;
        u64 x = u64(a.c) * b.d, y = u64(a.d) * b.c;
        return x != y ? x > y : a.id > b.id;
    };
    Buffer<Block> heap(n - 1);
    Buffer<u32> position(n), leader(n), next(n), tail(n);
    std::iota(leader.p, leader.p + n, 0u);
    std::iota(tail.p, tail.p + n, 0u);
    std::fill(next.p, next.p + n, ~0u);
    for (u32 v = 1; v < n; ++v) {
        heap[v - 1] = {c[v], d[v], v};
        position[v] = v - 1;
    }
    Block root{c[0], d[0], 0};
    auto down = [&](u32 at) {
        Block x = heap[at];
        for (u32 child; (child = 2 * at + 1) < heap.n;) {
            if (child + 1 < heap.n && better(heap[child + 1], heap[child])) ++child;
            if (!better(heap[child], x)) break;
            heap[at] = heap[child];
            position[heap[at].id] = at;
            at = child;
        }
        heap[at] = x;
        position[x.id] = at;
    };
    for (u32 at = heap.n / 2; at--;) down(at);
    auto find = [&](u32 v) {
        while (leader[v] != v) {
            leader[v] = leader[leader[v]];
            v = leader[v];
        }
        return v;
    };
    while (heap.n) {
        Block child = heap[0], last = heap[--heap.n];
        if (heap.n) {
            heap[0] = last;
            down(0);
        }
        u32 v = child.id, p = find(parent[v]);
        Block current = p ? heap[position[p]] : root;
        result.cost += u64(current.d) * child.c;
        current.c += child.c;
        current.d += child.d;
        // Merging the globally largest density can only increase the parent priority.
        if (p) {
            u32 at = position[p];
            while (at) {
                u32 up = (at - 1) / 2;
                if (!better(current, heap[up])) break;
                heap[at] = heap[up];
                position[heap[at].id] = at;
                at = up;
            }
            heap[at] = current;
            position[p] = at;
        } else
            root = current;
        leader[v] = p;
        next[tail[p]] = v;
        tail[p] = tail[v];
    }
    for (u32 i = 0, v = 0; i < n; ++i, v = next[v]) result.order[i] = v;
    return result;
}
} // namespace toy
