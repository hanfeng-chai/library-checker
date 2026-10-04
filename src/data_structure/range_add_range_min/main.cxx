#include <toy/io.h>
#include <toy/range_add_min.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>(), q = in.read<u32, 6>();
    Buffer<i64> a(n);
    for (auto &x : std::span(a.p, a.n)) x = in.read<i32, 10>();
    RangeAddMin tree{std::span<const i64>(a)};
    struct Op {
        u32 l, r;
        i32 value;
    };
    Buffer<Op> ops(q);
    for (auto &op : std::span(ops.p, ops.n)) {
        u32 type = in.read<u32, 1>();
        auto [l, r] = in.read_pair<6>();
        op = {l | (type << 31), r, type ? 0 : in.read<i32, 10>()};
    }
    for (usize i = 0; i < q; ++i) {
        if (i + 4 < q) tree.prefetch(ops[i + 4].l & 0x7fffffff, ops[i + 4].r);
        auto op = ops[i];
        if (op.l >> 31)
            out.write(tree.minimum(op.l & 0x7fffffff, op.r));
        else
            tree.add(op.l, op.r, op.value);
    }
}
