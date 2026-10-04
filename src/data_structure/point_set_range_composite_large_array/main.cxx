#include <toy/affine_tree.h>
#include <toy/io.h>
#include <toy/radix_sort.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    in.read<u32, 10>();
    u32 q = in.read<u32, 6>();
    struct Op {
        u32 type, left, right, a, b;
    };
    Buffer<Op> ops(q);
    Buffer<u64> events(0, 2 * q);
    u32 queries = 0, changes = 0;
    for (u32 i = 0; i < q; ++i) {
        auto &op = ops[i];
        op.type = in.read<u32, 1>();
        op.left = in.read<u32, 10>();
        if (!op.type) {
            op.right = 0;
            op.a = in.read<u32, 16>();
            op.b = in.read<u32, 16>();
            events[events.n++] = (u64(4 * i) << 32) | op.left;
            ++changes;
        } else {
            op.b = 0;
            op.right = in.read<u32, 10>();
            op.a = in.read<u32, 16>();
            events[events.n++] = (u64(4 * i + 1) << 32) | op.left;
            events[events.n++] = (u64(4 * i + 2) << 32) | op.right;
            ++queries;
        }
    }
    if (!queries) return 0;
    if (!changes) {
        for (auto op : std::span(ops.p, ops.n)) out.write(op.a);
        return 0;
    }
    radix_sort(std::span(events.p, events.n), [](u64 x) { return u32(x); });
    u32 previous = ~0u, size = 0;
    for (u64 event : std::span(events.p, events.n)) {
        u32 coordinate = event, tag = event >> 32;
        auto &op = ops[tag >> 2];
        if (tag & 3) {
            u32 index = size - (coordinate == previous);
            if (tag & 1)
                op.left = index;
            else
                op.right = index;
        } else {
            if (coordinate != previous) {
                ++size;
                previous = coordinate;
            }
            op.left = size - 1;
        }
    }
    Buffer<Affine<>> initial(size, 2 * size);
    std::fill(initial.p, initial.p + size, Affine<>{});
    AffineTree tree(std::move(initial));
    for (auto op : std::span(ops.p, ops.n))
        if (!op.type)
            tree.set(op.left, {op.a, op.b});
        else
            out.write(tree.apply(op.left, op.right, op.a));
}
