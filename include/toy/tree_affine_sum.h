#pragma once
#include <toy/buffer.h>
#include <toy/montgomery.h>
namespace toy {
// Sum of vertex values transported through symmetric affine edge maps, for every root.
template <u32 P = 998244353>
struct TreeAffineSum {
    using R = Montgomery<P>;
    struct Node {
        u32 parent = 0, degree = 0, a = 0, b = 0, sum = 0, size = 1, contribution = 0, total = 0;
    };
    Buffer<Node> nodes;
    explicit TreeAffineSum(std::span<const u32> values) : nodes(values.size()) {
        for (u32 i = 0; i < values.size(); ++i) {
            nodes[i] = {};
            nodes[i].sum = values[i];
        }
    }
    void add_edge(u32 u, u32 v, u32 a, u32 b) {
        a = R::encode(a);
        b = R::encode(b);
        auto &x = nodes[u];
        auto &y = nodes[v];
        ++x.degree;
        ++y.degree;
        x.parent ^= v;
        y.parent ^= u;
        x.a ^= a;
        y.a ^= a;
        x.b ^= b;
        y.b ^= b;
    }
    static u32 mapped(u32 sum, u32 size, u32 a, u32 b) {
        u64 z = u64(sum) * a + u64(size) * b;
        u32 x = (z + u64(u32(z) * Mod<P>::inverse) * P) >> 32;
        return std::min(x, x - 2 * P);
    }
    Buffer<u32> solve(u32 root = 0) {
        u32 n = nodes.n;
        Buffer<u32> answer(n), order(n);
        if (!n) return answer;
        u32 at = n;
        ++nodes[root].degree;
        for (u32 start = 0; start < n; ++start) {
            u32 v = start;
            while (v != root && nodes[v].degree == 1) {
                auto &x = nodes[v];
                u32 p = x.parent;
                auto &y = nodes[p];
                order[--at] = v;
                x.contribution = mapped(x.sum, x.size, x.a, x.b);
                y.sum = R::add(y.sum, x.contribution);
                y.size += x.size;
                x.degree = 0;
                --y.degree;
                y.parent ^= v;
                y.a ^= x.a;
                y.b ^= x.b;
                v = p;
            }
        }
        order[0] = root;
        nodes[root].total = nodes[root].sum;
        for (u32 i = 1; i < n; ++i) {
            auto &x = nodes[order[i]];
            u32 outside = R::subtract(nodes[x.parent].total, x.contribution);
            x.total = R::add(x.sum, mapped(outside, n - x.size, x.a, x.b));
        }
        for (u32 v = 0; v < n; ++v) answer[v] = std::min(nodes[v].total, nodes[v].total - P);
        return answer;
    }
};
} // namespace toy
