#pragma once
#include <toy/dsu.h>
#include <toy/rollback_dsu.h>
namespace toy {
// Edmonds' arborescence algorithm with lazy skew heaps and reversible cycle
// contractions. Empty parent means that some vertex is unreachable from root.
struct DirectedMST {
    struct Node {
        u32 left = ~0u, right = ~0u, from, to;
        i64 weight, lazy = 0;
    };
    struct Result {
        i64 cost = 0;
        Buffer<u32> parent;
    };
    Buffer<Node> node;
    Buffer<u32> heap;
    u32 used = 0;
    DirectedMST(u32 n, u32 m) : node(m), heap(n) { std::fill(heap.p, heap.p + n, ~0u); }
    void apply(u32 i, i64 delta) {
        if (i != ~0u) {
            node[i].weight -= delta;
            node[i].lazy += delta;
        }
    }
    void push(u32 i) {
        auto &x = node[i];
        apply(x.left, x.lazy);
        apply(x.right, x.lazy);
        x.lazy = 0;
    }
    u32 meld(u32 a, u32 b) {
        if (a == ~0u || b == ~0u) return a == ~0u ? b : a;
        if (node[a].weight > node[b].weight) std::swap(a, b);
        push(a);
        node[a].right = meld(node[a].right, b);
        std::swap(node[a].left, node[a].right);
        return a;
    }
    void add(u32 u, u32 v, i64 w) {
        if (u == v) return;
        node[used] = {~0u, ~0u, u, v, w, 0};
        heap[v] = meld(heap[v], used++);
    }
    Result solve(u32 root) {
        u32 n = heap.n;
        Result result;
        DSU forest(n);
        RollbackDSU contracted(n);
        Buffer<RollbackDSU::Change> history(0, n);
        Buffer<std::array<u32, 2>> cycles(0, n);
        Buffer<u32> chosen(n);
        for (u32 i = 0; i < n; ++i)
            if (i != root) {
                u32 v = i;
                for (;;) {
                    u32 e = heap[v];
                    if (e == ~0u) return {};
                    chosen[v] = e;
                    result.cost += node[e].weight;
                    apply(e, node[e].weight);
                    u32 w = contracted.leader(node[e].from);
                    if (forest.merge(v, w)) break;
                    u32 time = history.n;
                    for (;;) {
                        auto change = contracted.merge(v, w);
                        if (change.root < 0) break;
                        history.p[history.n++] = change;
                        u32 h = meld(heap[v], heap[w]);
                        v = contracted.leader(v);
                        heap[v] = h;
                        w = contracted.leader(node[chosen[w]].from);
                    }
                    cycles.p[cycles.n++] = {chosen[v], time};
                    while (heap[v] != ~0u && contracted.same(v, node[heap[v]].from)) {
                        u32 e = heap[v];
                        push(e);
                        heap[v] = meld(node[e].left, node[e].right);
                    }
                }
            }
        while (cycles.n) {
            auto [e, time] = cycles[--cycles.n];
            u32 v = contracted.leader(node[e].to);
            while (history.n > time) contracted.undo(history[--history.n]);
            u32 w = contracted.leader(node[chosen[v]].to);
            chosen[w] = std::exchange(chosen[v], e);
        }
        result.parent = Buffer<u32>(n);
        for (u32 v = 0; v < n; ++v) result.parent[v] = v == root ? root : node[chosen[v]].from;
        return result;
    }
};
} // namespace toy
