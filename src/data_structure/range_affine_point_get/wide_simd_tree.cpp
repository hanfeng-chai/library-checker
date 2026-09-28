#include <toy/io.hpp>
#include <toy/wide_affine.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    for (toy::u32& value : values)
        value = input.read_uniform<9, toy::u32>();

    static toy::WideAffinePointTree<mod, 500'000, 500'000> tree;
    tree.reset(values);
    while (query_count--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0) {
            toy::u32 left = input.read_uniform<6, toy::u32>();
            toy::u32 right = input.read_uniform<6, toy::u32>();
            toy::u32 b = input.read_uniform<9, toy::u32>();
            toy::u32 c = input.read_uniform<9, toy::u32>();
            tree.apply(left, right, {b, c});
        } else {
            tree.collect(input.read_uniform<6, toy::u32>());
        }
    }
    for (toy::u32 answer : tree.resolve())
        output.write_padded_u32(answer);
}
