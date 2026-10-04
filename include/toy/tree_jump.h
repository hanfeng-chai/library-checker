#pragma once
#include <toy/buffer.h>
namespace toy {
struct TreeJump {
    struct Node {
        u32 depth = 0, offset = 0, base = 0, mask = 0;
    };
    Buffer<Node> nodes;
    Buffer<u64> prefixes;
    Buffer<u32> vertex;
    u32 shape = 0, center = 0;
    explicit TreeJump(u32 n) : nodes(n), prefixes(0), vertex(n) {
        std::fill(nodes.p, nodes.p + n, Node{0, 0, 1, ~0u});
    }
    void add_edge(u32 u, u32 v) {
        ++nodes[u].depth;
        ++nodes[v].depth;
        nodes[u].offset ^= v;
        nodes[v].offset ^= u;
    }
    void build() {
        u32 n = nodes.n;
        if (!n) return;
        u32 leaf = 0;
        for (u32 v = 0; v < n; ++v) {
            if (nodes[v].depth > nodes[center].depth) center = v;
            if (nodes[v].depth == 1) leaf = v;
        }
        if (n > 2 && nodes[center].depth == n - 1) {
            shape = 2;
            return;
        }
        if (nodes[center].depth <= 2) {
            shape = 1;
            u32 v = leaf, previous = 0;
            for (u32 i = 0; i < n; ++i) {
                nodes[v].depth = i;
                vertex[i] = v;
                u32 next = nodes[v].offset ^ previous;
                previous = v;
                v = next;
            }
            return;
        }
        Buffer<u32> order(n);
        u32 at = n;
        ++nodes[0].depth;
        for (u32 start = 0; start < n; ++start) {
            u32 v = start;
            while (v && nodes[v].depth == 1) {
                u32 p = nodes[v].offset;
                order[--at] = v;
                nodes[p].base += nodes[v].base;
                if (nodes[p].mask == ~0u || nodes[v].base > nodes[nodes[p].mask].base)
                    nodes[p].mask = v;
                nodes[v].depth = 0;
                --nodes[p].depth;
                nodes[p].offset ^= v;
                v = p;
            }
        }
        order[0] = 0;
        struct Work {
            u32 end, heavy;
        };
        Buffer<Work> work(n);
        for (u32 v = 0; v < n; ++v) work[v] = {nodes[v].base, nodes[v].mask};
        prefixes = Buffer<u64>(9, std::max<usize>(9, n));
        std::fill(prefixes.p, prefixes.p + prefixes.n, u64(LLONG_MAX));
        prefixes[0] = 1;
        prefixes[1] = u64(1) << 32;
        nodes[0] = {1, 1, 0, n <= 32 ? 1u : 0u};
        vertex[0] = 0;
        // Reuse the peeled node words for query metadata; temporary words hold subtree ends and
        // heavy children.
        for (u32 i = 1; i < n; ++i) {
            u32 v = order[i], p = nodes[v].offset, size = work[v].end, depth = nodes[p].depth + 1;
            bool heavy = work[p].heavy == v;
            u32 parent_position =
                    nodes[p].base + (nodes[p].mask ? 31u - std::countl_zero(nodes[p].mask) : 0u),
                position = heavy ? parent_position + 1 : (work[p].end -= size);
            work[v].end = position + size;
            vertex[position] = v;
            Node x{depth, nodes[p].offset, position, 0};
            if (size <= 32) {
                if (nodes[p].mask) {
                    x.base = nodes[p].base;
                    x.mask = nodes[p].mask | (1u << (position - x.base));
                } else
                    x.mask = 1;
            } else if (!heavy) {
                u32 old = nodes[p].offset, length = prefixes[old - 1], at = prefixes.n + 1,
                    end = at + ((length + 8) & ~3u);
                if (end > prefixes.capacity)
                    prefixes.reserve(std::max<usize>(end, 2 * prefixes.capacity));
                prefixes.n = end;
                prefixes[at - 1] = length + 1;
                memcpy(prefixes.p + at, prefixes.p + old, 8 * length);
                prefixes[at + length] = (u64(depth) << 32) | position;
                std::fill(prefixes.p + at + length + 1, prefixes.p + end, u64(LLONG_MAX));
                x.offset = at;
            }
            nodes[v] = x;
        }
    }
    static u32 boundary(Node x) { return x.depth - std::popcount(x.mask); }
    static u32 mismatch(const u64 *a, const u64 *b) {
        for (u32 i = 0;; i += 4) {
            auto x = _mm256_loadu_si256((const __m256i *)(a + i)),
                 y = _mm256_loadu_si256((const __m256i *)(b + i));
            u32 bits = _mm256_movemask_pd(_mm256_castsi256_pd(_mm256_cmpeq_epi64(x, y)));
            if (bits != 15) return i + std::countr_one(bits);
        }
    }
    u32 lca_depth(Node x, Node y) const {
        u32 a = boundary(x), b = boundary(y);
        if (x.base == y.base && x.mask && y.mask) return a + std::popcount(x.mask & y.mask);
        u32 d = std::min(a, b);
        if (x.offset == y.offset) return d;
        const u64 *p = prefixes.p + x.offset;
        const u64 *q = prefixes.p + y.offset;
        u32 k = mismatch(p, q);
        return std::min(d, std::min(u32(p[k] >> 32) - 1, u32(q[k] >> 32) - 1));
    }
    u32 at_depth(Node x, u32 target) const {
        u32 b = boundary(x);
        if (target > b)
            return vertex[x.base + std::countr_zero(_pdep_u32(1u << (target - b - 1), x.mask))];
        // Compare four chain-start depths at once; the first larger one ends the predecessor
        // search.
        const u64 *p = prefixes.p + x.offset;
        auto limit = _mm256_set1_epi64x(u64(target + 1) << 32);
        for (u32 i = 0;; i += 4) {
            auto a = _mm256_loadu_si256((const __m256i *)(p + i));
            u32 bits = _mm256_movemask_pd(_mm256_castsi256_pd(_mm256_cmpgt_epi64(limit, a)));
            if (bits != 15) {
                u64 entry = p[i + std::countr_one(bits) - 1];
                return vertex[u32(entry) + target - u32(entry >> 32)];
            }
        }
    }
    i32 jump(u32 u, u32 v, u32 k) const {
        if (!k) return u;
        if (shape == 2) {
            u32 d = (u != center) + (v != center);
            return u == v || k > d ? -1 : i32(k == d ? v : center);
        }
        if (shape == 1) {
            u32 x = nodes[u].depth, y = nodes[v].depth, d = x > y ? x - y : y - x;
            return k > d ? -1 : i32(vertex[x < y ? x + k : x - k]);
        }
        Node x = nodes[u], y = nodes[v];
        if (k > x.depth + y.depth - 2) return -1;
        u32 c = lca_depth(x, y), a = x.depth - c, b = y.depth - c;
        if (k > a + b) return -1;
        return k <= a ? at_depth(x, x.depth - k) : at_depth(y, c + k - a);
    }
    void prefetch(u32 u, u32 v) const {
        if (shape != 2) {
            __builtin_prefetch(nodes.p + u);
            __builtin_prefetch(nodes.p + v);
        }
    }
};
} // namespace toy
