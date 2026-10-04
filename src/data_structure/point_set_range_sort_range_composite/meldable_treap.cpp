#include <toy/affine.hpp>
#include <toy/io.hpp>
#include <toy/sortable.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    using Function = toy::Affine<mod>;

    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<int> keys(n);
    std::vector<Function> functions(n);
    for (int i = 0; i < n; ++i) {
        keys[i] = input.read_uniform<10, toy::u32>();
        functions[i].a = input.read_uniform<9, toy::u32>();
        functions[i].b = input.read_uniform<9, toy::u32>();
    }

    toy::TreapSortableSegmentTree tree(keys, functions, Function{}, toy::ComposeAffine<mod>{});
    for (int query = 0; query < query_count; ++query) {
        int type = input.read_fixed<1, toy::u32>();
        int x = input.read_uniform<6, toy::u32>();
        int y = input.read_uniform<10, toy::u32>();
        if (type == 0) {
            toy::u32 a = input.read_uniform<9, toy::u32>();
            toy::u32 b = input.read_uniform<9, toy::u32>();
            tree.set(x, y, Function{a, b});
        } else if (type == 1) {
            toy::u32 value = input.read_uniform<9, toy::u32>();
            output.write_padded_u32(tree.fold(x, y)(value));
        } else if (type == 2) {
            tree.sort_ascending(x, y);
        } else {
            tree.sort_descending(x, y);
        }
    }
}
