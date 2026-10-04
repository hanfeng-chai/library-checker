#pragma once
#include <toy/page_buffer.h>
#include <toy/vertex_groups.h>
namespace toy {
// n,m < 2^21. An unvisited vertex caches (head, first target, next arc).
// Arc zero is the null link. Traversals consume these records in place.
struct PackedDigraph {
    static constexpr u32 Bits = 21, Mask = (1u << Bits) - 1;
    static constexpr u64 Open = 1ull << 63, Done = ~0ull;
    Buffer<u64> vertex, edge;
    u32 count = 0;
    PackedDigraph(u32 n, u32 m) : vertex(page_buffer<u64>(n)), edge(page_buffer<u64>(m + 1)) {
        std::fill(vertex.p, vertex.p + n, 0ull);
    }
    static u32 a(u64 x) { return x & Mask; }
    static u32 b(u64 x) { return (x >> Bits) & Mask; }
    static u32 c(u64 x) { return (x >> (2 * Bits)) & Mask; }
    static u64 pack(u32 x, u32 y, u32 z = 0) {
        return x | (u64(y) << Bits) | (u64(z) << (2 * Bits));
    }
    void add(u32 u, u32 v) {
        u32 previous = a(vertex[u]);
        edge[++count] = pack(previous, v);
        vertex[u] = pack(count, v, previous);
    }
    Buffer<u32> cycle() {
        u32 n = vertex.n, depth = 0;
        auto path = page_buffer<u64>(n + 1);
        for (u32 root = 0; root < n; ++root) {
            u32 u = root, v = root;
            for (;;) {
                u64 x = vertex[v];
                if (x >= Open) {
                    u32 start = x & Mask;
                    Buffer<u32> result;
                    result.p =
                        ::new (static_cast<void *>(std::exchange(vertex.p, nullptr))) u32[2 * n];
                    result.capacity = 2 * n;
                    vertex.n = vertex.capacity = 0;
                    result.n = depth - start + 1;
                    for (u32 i = 0; i < result.n; ++i) result[i] = b(path[start + i]) - 1;
                    return result;
                }
                if (x) {
                    vertex[v] = Open | (depth + 1);
                    __builtin_prefetch(edge.p + c(x));
                    path[++depth] = pack(c(x), a(x), v);
                    u = v;
                    v = b(x);
                    continue;
                }
                if (!depth) break;
                u64 f = path[depth];
                u32 next = a(f);
                if (next) {
                    u64 e = edge[next];
                    if (a(e)) {
                        u64 after = edge[a(e)];
                        __builtin_prefetch(vertex.p + b(after));
                        __builtin_prefetch(edge.p + a(after));
                    }
                    path[depth] = pack(a(e), next, u);
                    v = b(e);
                    continue;
                }
                vertex[u] = 0;
                v = u;
                --depth;
                if (depth) u = c(path[depth]);
            }
        }
        return {};
    }
    template <class Emit, class Finish>
    void visit_components(Emit emit, Finish finish) {
        u32 n = vertex.n, stack = 0;
        u64 number = Open;
        auto singleton = [&](u32 v) {
            vertex[v] = Done;
            emit(v);
            finish();
        };
        for (u32 root = 0; root < n; ++root) {
            u64 x = vertex[root];
            if (x >= Open) continue;
            if (!x) {
                singleton(root);
                continue;
            }
            u32 u = root, v = root, frame = 0;
            for (;;) {
                u64 y = vertex[v];
                if (y < Open) {
                    if (y) {
                        vertex[v] = number | 1;
                        number += 2;
                        u32 head = a(y);
                        __builtin_prefetch(edge.p + c(y));
                        edge[head] = pack(c(y), frame, u);
                        frame = head;
                        u = v;
                        v = b(y);
                        continue;
                    }
                    singleton(v);
                } else if ((y | 1) < vertex[u])
                    vertex[u] = y & ~1ull;
                u64 saved = edge[frame];
                u32 next = a(saved);
                if (next) {
                    u64 e = edge[next];
                    if (a(e)) {
                        u64 after = edge[a(e)];
                        __builtin_prefetch(vertex.p + b(after));
                        __builtin_prefetch(edge.p + a(after));
                    }
                    edge[next] = (e & Mask) | (saved & ~u64(Mask));
                    frame = next;
                    v = b(e);
                    continue;
                }
                if (vertex[u] & 1) {
                    u64 bound = vertex[u] - 1;
                    emit(u);
                    while (stack) {
                        u64 e = edge[stack];
                        u32 w = b(e);
                        if (vertex[w] < bound) break;
                        vertex[w] = Done;
                        emit(w);
                        stack = a(e);
                    }
                    finish();
                    vertex[u] = Done;
                    number = bound;
                } else {
                    edge[frame] = pack(stack, u);
                    stack = frame;
                }
                u32 parent = c(saved);
                if (parent == u) break;
                v = u;
                u = parent;
                frame = b(saved);
            }
        }
    }
    Buffer<u32> component_ids() {
        Buffer<u32> id(vertex.n);
        u32 count = 0;
        visit_components([&](u32 v) { id[v] = count; }, [&] { ++count; });
        return id;
    }
    VertexGroups components() {
        u32 n = vertex.n;
        VertexGroups groups(n, n);
        visit_components([&](u32 v) { groups.add(v); }, [&] { groups.finish(); });
        std::reverse(groups.vertex.p, groups.vertex.p + n);
        std::reverse(groups.offset.p, groups.offset.p + groups.offset.n);
        for (auto &x : std::span(groups.offset.p, groups.offset.n)) x = n - x;
        return groups;
    }
};
} // namespace toy
