#pragma once
#include <toy/common.h>
namespace toy {
struct MSTEdge {
    u32 u, v, weight, id;
};
struct MSTView {
    u64 cost;
    std::span<u32> edge;
};
// Caller-owned work memory. Sorting consumes input; afterward scratch is
// reused for union-find and the returned IDs. Capacity: max(m,ceil((2n-1)/4)).
[[gnu::always_inline]] inline MSTView kruskal_workspace(u32 n, std::span<MSTEdge> input,
                                                        std::span<MSTEdge> scratch) {
    u32 m = input.size(), count[4][256]{};
    MSTEdge *from = input.data(), *to = scratch.data();
    for (u32 i = 0; i < m; ++i) {
        u32 w = from[i].weight;
#pragma GCC unroll 4
        for (u32 d = 0; d < 4; ++d) ++count[d][(w >> (d * 8)) & 255];
    }
    bool same = m && count[0][from[0].weight & 255] == m &&
                count[1][(from[0].weight >> 8) & 255] == m &&
                count[2][(from[0].weight >> 16) & 255] == m && count[3][from[0].weight >> 24] == m;
    if (m && !same) {
        auto pass = [&]<u32 Shift>(MSTEdge *from, MSTEdge *to) {
            u32 *row = count[Shift / 8];
            u32 sum = 0;
            for (u32 j = 0; j < 256; ++j) {
                u32 x = row[j];
                row[j] = sum;
                sum += x;
            }
            for (u32 i = 0; i < m; ++i) {
                u32 at = row[(from[i].weight >> Shift) & 255]++;
                if (at + 16 < m) __builtin_prefetch(to + at + 16, 1, 1);
                to[at] = from[i];
            }
        };
        pass.template operator()<0>(input.data(), scratch.data());
        pass.template operator()<8>(scratch.data(), input.data());
        pass.template operator()<16>(input.data(), scratch.data());
        pass.template operator()<24>(scratch.data(), input.data());
    }
    i32 *parent = ::new (static_cast<void *>(scratch.data())) i32[n];
    u32 *ids = ::new (static_cast<void *>(parent + n)) u32[n ? n - 1 : 0];
    std::fill(parent, parent + n, -1);
    auto leader = [&](u32 v) {
        while (parent[v] >= 0) {
            u32 p = parent[v];
            if (parent[p] < 0) return p;
            v = parent[v] = parent[p];
        }
        return v;
    };
    u32 used = 0;
    u64 total = 0;
    for (u32 i = 0; i < m; ++i) {
        if (i + 16 < m) {
            __builtin_prefetch(parent + input[i + 16].u, 1);
            __builtin_prefetch(parent + input[i + 16].v, 1);
        }
        auto e = input[i];
        u32 a = leader(e.u), b = leader(e.v);
        if (a == b) continue;
        if (parent[a] > parent[b]) std::swap(a, b);
        parent[a] += parent[b];
        parent[b] = a;
        total += e.weight;
        ids[used++] = e.id;
        if (used + 1 == n) break;
    }
    return {total, {ids, used}};
}
} // namespace toy
