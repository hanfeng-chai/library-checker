#include <toy/io.hpp>
#include <toy/range.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<toy::u64> values(n);
    for (auto &value : values) value = input.read_var<10, toy::u64>();
    toy::SegmentTree<toy::u64, std::plus<>> tree(values, 0);
    while (queries--) {
        int left = input.read_uniform<6, toy::u32>();
        int right = input.read_uniform<6, toy::u32>();
        output.writeln(tree.fold(left, right));
    }
}
