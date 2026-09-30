#pragma once
#include <toy/affine.h>
#include <toy/buffer.h>
#include <toy/montgomery.h>

namespace toy {
// Reverse the leaf order in a compact 2*n tree. Queries apply each canonical
// segment directly to x, avoiding construction of a second aggregate function.
template<u32 P = 998244353> struct AffineTree {
    using Function = Affine<P>;using R=Montgomery<P>;
    static Function combine(Function first,Function second){return {R::multiply(first.a,second.a),R::add(R::multiply(first.b,second.a),second.b)};}
    static u32 evaluate(Function f,u32 x){return R::add(R::multiply(f.a,x),f.b);}
    usize n;
    Buffer<Function> tree;
    explicit AffineTree(Buffer<Function> values) : n(values.n), tree(std::move(values)) {
        tree.reserve(2 * n); tree.n = 2 * n;
        for (usize i = 0; i < n; ++i) tree[2 * n - 1 - i] = {R::encode(tree[i].a),tree[i].b};
        for (usize i = n; i-- > 1;) tree[i] = combine(tree[2 * i + 1], tree[2 * i]);
    }
    void set(usize i, Function f) {
        usize at = 2 * n - 1 - i; tree[at] = {R::encode(f.a),f.b};
        while (at >>= 1) tree[at] = combine(tree[2 * at + 1], tree[2 * at]);
    }
    u32 apply(u32 left, u32 right, u32 x) const {
        if (left == right) return x;
        u32 l = 2 * n - 1 - right, r = 2 * n - left, width = bit_width(l ^ r) - 1, boundary = r >> width;
        for (r = (r >> countr_zero(r)) ^ 1; r > boundary; r = (r >> countr_zero(r)) ^ 1) x = evaluate(tree[r],x);
        u32 rest = ~l & ((1u << width) - 1);
        while (rest) { unsigned k = bit_width(rest) - 1; rest ^= 1u << k; x = evaluate(tree[(l >> k) ^ 1],x); }
        return min(x,x-P);
    }
};
}
