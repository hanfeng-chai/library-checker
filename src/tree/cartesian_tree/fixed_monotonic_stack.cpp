#include <toy/io.hpp>
#include <toy/tree.hpp>

int main() {
    static toy::FixedCartesianTree<uint32_t, 1'000'000> tree;
    toy::Reader input(toy::direct_mapping); toy::Writer<1 << 20> output;
    int n = input.read_uniform<7, uint32_t>();
    for (int i = 0; i < n; ++i) tree[i] = input.read_uniform<10, uint32_t>();
    const int* parent = tree.build(n);
    for (int vertex = 0; vertex < n; ++vertex)
        output.write_token_u32_6(parent[vertex]);
    output.put('\n');
}
