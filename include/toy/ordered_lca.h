#pragma once
#include <toy/buffer.h>
namespace toy {
// Schieber-Vishkin labels for a rooted tree numbered with parent[v] < v; parent[0]=0.
struct OrderedLCA {
    struct Label {
        u32 path, ascendant;
    };
    Buffer<Label> label;
    Buffer<u32> head_parent;
    static u32 lowbit(u32 x) { return x & -x; }
    explicit OrderedLCA(std::span<const u32> parent)
        : label(parent.size()), head_parent(parent.size() + 1) {
        u32 n = parent.size();
        if (!n) return;
        Buffer<u32> leaves(n);
        std::fill(leaves.p, leaves.p + n, 0u);
        for (u32 v = n; --v;) {
            if (!leaves[v]) leaves[v] = 1;
            leaves[parent[v]] += leaves[v];
        }
        label[0].path = 1;
        for (u32 v = 1; v < n; ++v) {
            label[v].path = label[parent[v]].path;
            label[parent[v]].path += leaves[v];
        }
        for (u32 v = 0; v < n; ++v) label[parent[v]].path = 0;
        for (u32 v = n; v--;) {
            u32 p = parent[v];
            if (lowbit(label[p].path) < lowbit(label[v].path)) label[p].path = label[v].path;
        }
        for (u32 v = n; v--;) head_parent[label[v].path] = parent[v];
        label[0].ascendant = 0;
        for (u32 v = 1; v < n; ++v)
            label[v].ascendant = label[parent[v]].ascendant | lowbit(label[v].path);
    }
    u32 lca(u32 a, u32 b) const {
        u32 difference = label[a].path ^ label[b].path;
        if (difference) {
            u32 common = label[a].ascendant & label[b].ascendant & -std::bit_floor(difference);
            // Strip each endpoint to the head of its last path below the shared path.
            u32 branch = label[a].ascendant ^ common;
            if (branch) {
                u32 bit = std::bit_floor(branch);
                a = head_parent[(label[a].path & -bit) | bit];
            }
            branch = label[b].ascendant ^ common;
            if (branch) {
                u32 bit = std::bit_floor(branch);
                b = head_parent[(label[b].path & -bit) | bit];
            }
        }
        return std::min(a, b);
    }
};
} // namespace toy
