#include <toy/affine.hpp>
#include <toy/io.hpp>
#include <toy/range.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    using Function = toy::Affine<mod>;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<Function> identities(queries);
    toy::SegmentTree tree(identities, Function{}, toy::ComposeAffine<mod>{});
    int front = 0;
    int back = 0;
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0) {
            toy::u32 a = input.read_uniform<9, toy::u32>();
            toy::u32 b = input.read_uniform<9, toy::u32>();
            tree.set(back++, {a, b});
        } else if (type == 1) {
            tree.set(front++, {});
        } else {
            toy::u32 x = input.read_uniform<9, toy::u32>();
            output.write_padded_u32(tree.fold(front, back)(x));
        }
    }
}
