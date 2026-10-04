#include <toy/io.hpp>
#include <toy/tree.hpp>

int main() {
    static toy::FixedOfflineTreeJump<500'000, 500'000> tree;
    toy::Reader input(toy::direct_mapping);
    toy::Writer<1 << 20> output;
    int n = input.read_uniform<6, uint32_t>();
    int q = input.read_uniform<6, uint32_t>();
    tree.reset(n, q);
    for (int i = 1; i < n; ++i) {
        int from = input.read_uniform<6, uint32_t>();
        int to = input.read_uniform<6, uint32_t>();
        tree.add_edge(from, to);
    }
    tree.build();
    for (int index = 0; index < q; ++index) {
        uint32_t from = input.read_uniform<6, uint32_t>();
        uint32_t to = input.read_uniform<6, uint32_t>();
        uint32_t distance = input.read_uniform<6, uint32_t>();
        tree.add_query(index, from, to, distance);
    }
    const uint32_t *answers = tree.solve();
    for (int index = 0; index < q; ++index) {
        if (answers[index] == std::numeric_limits<uint32_t>::max())
            output.write(" -1");
        else
            output.write_token_u32_6(answers[index]);
    }
}
