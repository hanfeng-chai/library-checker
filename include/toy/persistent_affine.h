#pragma once
#include <toy/affine.h>
#include <toy/buffer.h>
#include <toy/montgomery.h>
namespace toy {
// Immutable binary trees; sums are ordinary residues, lazy maps Montgomery-encoded.
template <u32 P = 998244353>
struct PersistentAffineArray {
    using R = Montgomery<P>;
    using Function = Affine<P>;
    struct Node {
        u32 left = 0, right = 0, sum = 0;
        Function lazy{R::one, 0};
    };
    Buffer<Node> nodes;
    u32 n, root;
    static Function identity() { return {R::one, 0}; }
    static bool is_identity(Function f) { return f.a == R::one && !f.b; }
    static Function compose(Function first, Function second) {
        return {R::multiply(first.a, second.a), R::add(R::multiply(first.b, second.a), second.b)};
    }
    static u32 mapped(u32 sum, u32 count, Function f) {
        u64 z = u64(sum) * f.a + u64(count) * f.b;
        u32 value = (z + u64(u32(z) * Mod<P>::inverse) * P) >> 32;
        return std::min(value, value - 2 * P);
    }
    u32 create(Node node) {
        if (nodes.n == nodes.capacity) nodes.reserve(std::max<usize>(4, 2 * nodes.capacity));
        nodes[nodes.n] = node;
        return nodes.n++;
    }
    u32 merge(u32 a, u32 b) { return create({a, b, R::add(nodes[a].sum, nodes[b].sum)}); }
    u32 build(std::span<const u32> a, u32 l, u32 r) {
        if (r - l == 1) return create({0, 0, a[l]});
        u32 m = (l + r) / 2, x = build(a, l, m), y = build(a, m, r);
        return merge(x, y);
    }
    explicit PersistentAffineArray(std::span<const u32> values, usize reserve = 0)
        : nodes(1, std::max<usize>(2 * values.size() + 1, reserve)), n(values.size()) {
        nodes[0] = {};
        root = n ? build(values, 0, n) : 0;
    }
    u32 transform(u32 old, u32 length, Function f) {
        if (is_identity(f)) return old;
        Node node = nodes[old];
        node.sum = mapped(node.sum, length, f);
        node.lazy = compose(node.lazy, f);
        return create(node);
    }
    u32 update(u32 old, u32 l, u32 r, u32 first, u32 last, Function carry, Function f) {
        if (last <= l || r <= first) return transform(old, r - l, carry);
        if (first <= l && r <= last) return transform(old, r - l, compose(carry, f));
        Node node = nodes[old];
        carry = compose(node.lazy, carry);
        u32 m = (l + r) / 2;
        u32 a = update(node.left, l, m, first, last, carry, f),
            b = update(node.right, m, r, first, last, carry, f);
        return merge(a, b);
    }
    u32 copy(u32 destination, u32 source, u32 l, u32 r, u32 first, u32 last, Function to,
             Function from) {
        if (last <= l || r <= first || (destination == source && to.a == from.a && to.b == from.b))
            return transform(destination, r - l, to);
        if (first <= l && r <= last) return transform(source, r - l, from);
        Node a = nodes[destination], b = nodes[source];
        to = compose(a.lazy, to);
        from = compose(b.lazy, from);
        u32 m = (l + r) / 2;
        u32 x = copy(a.left, b.left, l, m, first, last, to, from),
            y = copy(a.right, b.right, m, r, first, last, to, from);
        return merge(x, y);
    }
    u32 query(u32 node, u32 l, u32 r, u32 first, u32 last) const {
        const auto &x = nodes[node];
        if (first <= l && r <= last) return x.sum;
        u32 m = (l + r) / 2, sum;
        if (last <= m)
            sum = query(x.left, l, m, first, last);
        else if (first >= m)
            sum = query(x.right, m, r, first, last);
        else
            sum = R::add(query(x.left, l, m, first, last), query(x.right, m, r, first, last));
        return is_identity(x.lazy) ? sum
                                   : mapped(sum, std::min(r, last) - std::max(l, first), x.lazy);
    }
    u32 apply(u32 version, u32 l, u32 r, Function f) {
        if (l == r) return version;
        return update(version, 0, n, l, r, identity(), {R::encode(f.a), R::encode(f.b)});
    }
    u32 copy(u32 destination, u32 source, u32 l, u32 r) {
        if (l == r || destination == source) return destination;
        return copy(destination, source, 0, n, l, r, identity(), identity());
    }
    u32 sum(u32 version, u32 l, u32 r) const {
        if (l == r) return 0;
        u32 value = query(version, 0, n, l, r);
        return std::min(value, value - P);
    }
};
} // namespace toy
