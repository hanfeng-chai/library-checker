#include <toy/io.hpp>
#include <toy/tree.hpp>

int main() {
    static toy::FixedWeightedTreeDiameter<500'000> tree;
    toy::Reader input(toy::direct_mapping);
    toy::Writer<1 << 20> output;
    int n = input.read_uniform<6, uint32_t>();
    tree.reset(n);
    for (int i = 1; i < n; ++i) {
        int from = input.read_uniform<6, uint32_t>();
        int to = input.read_uniform<6, uint32_t>();
        uint32_t weight = input.read_uniform<10, uint32_t>();
        tree.add_edge(from, to, weight);
    }
    auto diameter = tree.solve();
    output.write_token((toy::u64)diameter.length);
    output.write_token_u32_6(diameter.path.size());
    for (int vertex : diameter.path) output.write_token_u32_6(vertex);
    output.put('\n');
}
