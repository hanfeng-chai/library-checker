#include <toy/io.hpp>
#include <toy/tree.hpp>

int main() {
    static toy::FixedHeavyLightTree<500'000> tree;
    toy::Reader input(toy::direct_mapping);
    toy::Writer<1 << 20> output;
    int n = input.read_uniform<6, uint32_t>();
    int q = input.read_uniform<6, uint32_t>();
    tree.reset(n);
    for (int edge = 1; edge < n; ++edge) {
        int from = input.read_uniform<6, uint32_t>();
        int to = input.read_uniform<6, uint32_t>();
        tree.add_edge(from, to);
    }
    tree.build();
    while (q--) {
        int from = input.read_uniform<6, uint32_t>();
        int to = input.read_uniform<6, uint32_t>();
        int step = input.read_uniform<6, uint32_t>();
        int answer = tree.jump(from, to, step);
        if (answer < 0)
            output.write(" -1");
        else
            output.write_token_u32_6(answer);
    }
}
