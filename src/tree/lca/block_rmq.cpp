#include <toy/io.hpp>
#include <toy/tree.hpp>

int main() {
    static toy::FixedOrderedParentLca<500'000, 10> tree;
    static int parents[499'999];
    toy::Reader input(toy::direct_mapping); toy::Writer<1 << 20> output;
    int n = input.read_uniform<6, uint32_t>();
    int q = input.read_uniform<6, uint32_t>();
    for (int i = 0; i + 1 < n; ++i) parents[i] = input.read_uniform<6, uint32_t>();
    tree.build(n, parents);
    while (q--) {
        int first = input.read_uniform<6, uint32_t>();
        int second = input.read_uniform<6, uint32_t>();
        output.write_token_u32_6(tree.lca(first, second));
    }
}
