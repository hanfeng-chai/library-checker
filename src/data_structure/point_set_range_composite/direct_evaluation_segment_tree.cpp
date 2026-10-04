#include <toy/affine.hpp>
#include <toy/io.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    using Function = toy::Affine<mod>;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<Function> functions(n);
    for (auto &function : functions)
        function = {input.read_uniform<9, toy::u32>(), input.read_uniform<9, toy::u32>()};
    toy::AffineEvaluationSegmentTree<mod> tree(functions);
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0) {
            int index = input.read_uniform<6, toy::u32>();
            toy::u32 a = input.read_uniform<9, toy::u32>();
            toy::u32 b = input.read_uniform<9, toy::u32>();
            tree.set(index, {a, b});
        } else {
            int left = input.read_uniform<6, toy::u32>();
            int right = input.read_uniform<6, toy::u32>();
            toy::u32 x = input.read_uniform<9, toy::u32>();
            output.write_padded_u32(tree.apply(left, right, x));
        }
    }
}
