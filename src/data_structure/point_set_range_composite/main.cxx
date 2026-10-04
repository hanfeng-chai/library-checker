#include <toy/affine_prefix.h>
#include <toy/io.h>
using namespace toy;
int main() {
    using Tree = AffinePrefixTree<>;
    using R = Tree::R;
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), q = in.read<u32, 6>();
    Buffer<Affine<>> values(n);
    for (auto &f : std::span(values.p, values.n)) {
        auto [a, b] = in.read_pair<9>();
        f = {R::encode(a), R::encode(b)};
    }
    struct Op {
        u32 position, a, b, inverse;
    };
    Buffer<Op> ops(q);
    Buffer<u32> updates(q);
    u32 count = 0, product = R::one;
    for (auto &op : std::span(ops.p, ops.n)) {
        u32 type = in.read<u32, 1>();
        op.position = in.read<u32, 6>();
        auto [first, second] = in.read_pair<9>();
        if (type) {
            op.position |= 1u << 31;
            op.a = first;
            op.b = second;
        } else {
            op.a = R::encode(first);
            op.b = R::encode(second);
            op.inverse = product;
            product = R::multiply(product, op.a);
            updates[count++] = &op - ops.p;
        }
    }
    u32 inverse = R::power(product, 998244351);
    for (u32 i = count; i--;) {
        auto &op = ops[updates[i]];
        u32 x = R::multiply(inverse, op.inverse);
        inverse = R::multiply(inverse, op.a);
        op.inverse = x;
    }
    Tree tree{std::span<const Affine<>>(values)};
    for (u32 i = 0; i < count; ++i) {
        if (i + 12 < count) __builtin_prefetch(values.p + ops[updates[i + 12]].position, 0, 3);
        auto &op = ops[updates[i]];
        auto old = values[op.position];
        values[op.position] = {op.a, op.b};
        op.a = R::multiply(old.a, op.inverse);
        op.b = R::multiply(R::subtract(old.b, op.b), op.inverse);
        op.inverse = R::subtract(R::one, op.a);
    }
    struct Answer {
        u32 numerator, denominator, prefix;
    };
    Buffer<Answer> answers(q - count);
    count = 0;
    product = R::one;
    for (u32 i = 0; i < q; ++i) {
        if (i + 4 < q) {
            auto next = ops[i + 4];
            tree.prefetch(next.position & 0x7fffffff, next.position >> 31 ? next.a : next.position);
        }
        auto op = ops[i];
        if (op.position >> 31) {
            auto f = tree.query(op.position & 0x7fffffff, op.a, op.b);
            answers[count++] = {f.numerator, f.denominator, product};
            product = R::multiply(product, f.denominator);
        } else
            tree.change(op.position, op.a, op.b, op.inverse);
    }
    inverse = R::decode(R::power(product, 998244351));
    for (u32 i = count; i--;) {
        auto &x = answers[i];
        u32 d = R::multiply(inverse, x.prefix);
        inverse = R::multiply(inverse, x.denominator);
        u32 value = R::multiply(x.numerator, d);
        x.numerator = std::min(value, value - 998244353);
    }
    for (auto x : std::span(answers.p, count)) out.write(x.numerator);
}
