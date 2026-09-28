#include <toy/affine.hpp>
#include <toy/io.hpp>
#include <toy/range.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    using Function = toy::Affine<mod>;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<Function> identities(2 * queries + 1);
    toy::SegmentTree tree(identities, Function{}, toy::ComposeAffine<mod>{});
    int left = queries;
    int right = queries;
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type <= 1) {
            Function function{input.read_uniform<9, toy::u32>(),
                              input.read_uniform<9, toy::u32>()};
            if (type == 0) tree.set(--left, function);
            else tree.set(right++, function);
        } else if (type == 2) {
            tree.set(left++, {});
        } else if (type == 3) {
            tree.set(--right, {});
        } else {
            toy::u32 x = input.read_uniform<9, toy::u32>();
            output.write_padded_u32(tree.fold(left, right)(x));
        }
    }
}
