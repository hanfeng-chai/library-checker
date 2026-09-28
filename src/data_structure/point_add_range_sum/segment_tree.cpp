#include <toy/io.hpp>
#include <toy/range.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::CompactWriter<> output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<toy::u64> values(n);
    for (auto& value : values) value = input.read_uniform<10, toy::u64>();
    toy::SegmentTree<toy::u64, std::plus<>> tree(values, 0);
    while (queries--) {
        int type = input.read_fixed<1, toy::u32>();
        int x = input.read_uniform<6, toy::u32>();
        if (type == 0) {
            toy::u64 increment = input.read_uniform<10, toy::u64>();
            tree.set(x, tree.get(x) + increment);
        } else {
            int right = input.read_uniform<6, toy::u32>();
            output.writeln(tree.fold(x, right));
        }
    }
}
