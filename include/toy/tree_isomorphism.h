#pragma once
#include <toy/radix_sort.h>
namespace toy {
struct ChildrenHash {
    u64 operator()(std::span<const u32> children) const {
        u64 h = 0x9e3779b97f4a7c15ull + children.size();
        for (u32 x : children) h = (h ^ x) * 0xbf58476d1ce4e5b9ull;
        return (h ^ (h >> 31)) * 0x94d049bb133111ebull;
    }
};
struct RootedClasses {
    u32 count;
    Buffer<u32> color;
};
// parent[0]=0, parent[v]<v. Hashes accelerate lookup; equal child lists are still compared exactly.
template <class Hash = ChildrenHash>
RootedClasses rooted_classes(std::span<const u32> parent, Hash hash = {}) {
    u32 n = parent.size();
    RootedClasses result{0, Buffer<u32>(n)};
    if (!n) return result;
    Buffer<u32> offset(n + 1), cursor(n), children(n - 1);
    std::fill(offset.p, offset.p + offset.n, 0u);
    for (u32 v = 1; v < n; ++v) ++offset[parent[v] + 1];
    for (u32 v = 1; v <= n; ++v) offset[v] += offset[v - 1];
    memcpy(cursor.p, offset.p, 4 * n);
    for (u32 v = 1; v < n; ++v) children[cursor[parent[v]]++] = v;
    u32 branching = 0;
    for (u32 v = 0; v < n; ++v) branching += offset[v + 1] - offset[v] >= 2;
    Buffer<u64> table(std::bit_ceil(std::max(4u, 2 * branching + 1)));
    std::fill(table.p, table.p + table.n, ~u64(0));
    u32 mask = table.n - 1, shift = 64 - std::countr_zero(u64(table.n));
    auto &unary = cursor;
    unary[0] = ~0u;
    result.count = 1;
    for (u32 v = n; v--;) {
        u32 first = offset[v], count = offset[v + 1] - first;
        if (!count) {
            result.color[v] = 0;
            continue;
        }
        if (count == 1) {
            u32 child = result.color[children[first]], id = unary[child];
            if (id == ~0u) {
                id = unary[child] = result.count++;
                unary[id] = ~0u;
            }
            result.color[v] = id;
            continue;
        }
        std::span<u32> list{children.p + first, count};
        for (auto &child : list) child = result.color[child];
        radix_sort(list, [](u32 x) { return x; });
        u64 h = hash(std::span<const u32>(list));
        u32 fingerprint = h >> 32;
        u32 at = ((h ^ (h >> 32)) * 11995408973635179863ull) >> shift;
        while (table[at] != ~u64(0)) {
            u32 u = table[at];
            if (u32(table[at] >> 32) == fingerprint && offset[u + 1] - offset[u] == count &&
                std::equal(list.begin(), list.end(), children.p + offset[u]))
                break;
            at = (at + 1) & mask;
        }
        if (table[at] == ~u64(0)) {
            table[at] = u64(fingerprint) << 32 | v;
            result.color[v] = result.count;
            unary[result.count++] = ~0u;
        } else
            result.color[v] = result.color[u32(table[at])];
    }
    return result;
}
} // namespace toy
