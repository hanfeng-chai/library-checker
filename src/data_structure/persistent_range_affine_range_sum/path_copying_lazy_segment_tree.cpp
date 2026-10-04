#include <toy/io.hpp>
#include <toy/persistent.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    for (toy::u32 &value : values) value = input.read_uniform<9, toy::u32>();

    toy::PersistentAffineArray<mod> tree(values,
                                         (std::size_t)n * 2 + (std::size_t)query_count * 80);
    std::vector<int> roots(query_count + 1);
    roots[0] = tree.root;
    for (int query = 0; query < query_count; ++query) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int base = input.read_uniform<6, int>() + 1;
        if (type == 0) {
            int left = input.read_uniform<6, toy::u32>();
            int right = input.read_uniform<6, toy::u32>();
            toy::u32 multiplier = input.read_uniform<9, toy::u32>();
            toy::u32 addend = input.read_uniform<9, toy::u32>();
            roots[query + 1] = tree.apply(roots[base], left, right, multiplier, addend);
        } else if (type == 1) {
            int source = input.read_uniform<6, int>() + 1;
            int left = input.read_uniform<6, toy::u32>();
            int right = input.read_uniform<6, toy::u32>();
            roots[query + 1] = tree.copy(roots[base], roots[source], left, right);
        } else {
            int left = input.read_uniform<6, toy::u32>();
            int right = input.read_uniform<6, toy::u32>();
            roots[query + 1] = roots[base];
            output.write_padded_u32(tree.fold(roots[base], left, right));
        }
    }
}
