#include <toy/io.hpp>
#include <toy/range.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, int>(), q = input.read_uniform<6, int>();
    std::vector<int64_t> values(n);
    for (auto& value : values) value = input.read_uniform<10, int64_t>();
    toy::RangeAddMinTree tree(values);
    while (q--) {
        int type = input.read_fixed<1, int>();
        int left = input.read_uniform<6, int>(), right = input.read_uniform<6, int>();
        if (!type) tree.add(left, right, input.read_uniform<10, int64_t>());
        else output.writeln_i64(tree.minimum_of(left, right));
    }
}
