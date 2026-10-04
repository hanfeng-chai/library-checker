#include <toy/affine.h>
#include <toy/io.h>
#include <toy/sortable.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), q = in.read<u32, 6>();
    Buffer<u32> keys(n);
    Buffer<Affine<>> values(n);
    for (u32 i = 0; i < n; ++i) {
        keys[i] = in.read<u32, 10>();
        values[i] = {in.read<u32, 16>(), in.read<u32, 16>()};
    }
    SortableSequence tree{std::span<const u32>(keys), std::span<const Affine<>>(values), Affine<>{},
                          ComposeAffine<>{}};
    while (q--) {
        u32 type = in.read<u32, 1>();
        if (!type) {
            u32 i = in.read<u32, 6>(), key = in.read<u32, 10>(), a = in.read<u32, 16>(),
                b = in.read<u32, 16>();
            tree.set(i, key, {a, b});
        } else {
            auto [l, r] = in.read_pair<6>();
            if (type == 1) {
                u32 x = in.read<u32, 16>();
                out.write(tree.fold(l, r)(x));
            } else
                tree.sort(l, r, type == 3);
        }
    }
}
