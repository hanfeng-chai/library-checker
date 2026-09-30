#pragma once
#include <toy/buffer.h>

namespace toy {
template<class T> Buffer<T> version_storage(usize n) {
    if (n * sizeof(T) < (1 << 20)) return Buffer<T>(n);
    usize bytes = (n * sizeof(T) + (1 << 21) - 1) & -usize(1 << 21);
    Buffer<T> b; b.p = (T*)aligned_alloc(1 << 21, bytes); b.n = b.capacity = n;
    madvise(b.p, bytes, MADV_HUGEPAGE); return b;
}

// Version zero is the initial state. add(base) creates the next numbered child.
// Enter/leave callbacks allow offline persistence with reversible operations.
struct VersionTree {
    Buffer<int> head, next;
    int count = 0;
    explicit VersionTree(usize capacity) : head(version_storage<int>(capacity + 1)), next(version_storage<int>(capacity + 1)) { fill(head.p, head.p + head.n, 0); }
    int add(int base) { ++count; next[count] = head[base]; return head[base] = count; }
    template<class Enter, class Leave> void visit(Enter enter, Leave leave) const {
        Buffer<int> stack(2 * count + 1); usize size = 0;
        if (head[0]) stack[size++] = head[0];
        while (size) {
            int u = stack[--size];
            if (u < 0) { leave(~u); continue; }
            if (next[u]) stack[size++] = next[u];
            enter(u); stack[size++] = ~u;
            if (head[u]) stack[size++] = head[u];
        }
    }
};
}
