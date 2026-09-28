#include <toy/io.hpp>
#include <toy/tree.hpp>

[[gnu::optimize("unroll-loops")]] int main() {
    static toy::FixedSchieberVishkinLca<500'000> tree;
    toy::Reader input(toy::direct_mapping); toy::Writer<1 << 20> output;
    int n = input.read_uniform<6, uint32_t>();
    int q = input.read_uniform<6, uint32_t>();
    tree.reset(n);
    for (int vertex = 1; vertex < n; ++vertex)
        tree.add_parent(vertex, input.read_uniform<6, uint32_t>());
    tree.build();
    while (q--) {
        int first = input.read_uniform<6, uint32_t>();
        int second = input.read_uniform<6, uint32_t>();
        output.write_token_u32_6(tree.lca(first, second));
    }
}
