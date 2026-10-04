#include <toy/ds.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::CompactWriter<> output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<toy::u64> values(n);
    for (auto &value : values) value = input.read_uniform<10, toy::u64>();
    toy::FenwickTree<toy::u64> tree(values);
    while (queries--) {
        int type = input.read_fixed<1, toy::u32>();
        int x = input.read_uniform<6, toy::u32>();
        if (type == 0)
            tree.add(x, input.read_uniform<10, toy::u64>());
        else
            output.writeln(tree.sum(x, input.read_uniform<6, toy::u32>()));
    }
}
