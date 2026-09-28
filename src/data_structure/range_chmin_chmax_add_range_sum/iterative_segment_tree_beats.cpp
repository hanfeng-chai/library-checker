#include <toy/io.hpp>
#include <toy/range.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, int>();
    int q = input.read_uniform<6, int>();
    std::vector<int64_t> values(n);
    for (auto& value : values) value = input.read_uniform<13, int64_t>();
    toy::RangeClampAddSumTree tree(values);
    while (q--) {
        int type = input.read_fixed<1, int>();
        int left = input.read_uniform<6, int>();
        int right = input.read_uniform<6, int>();
        if (type == 0) tree.chmin(left, right, input.read_uniform<13, int64_t>());
        else if (type == 1) tree.chmax(left, right, input.read_uniform<13, int64_t>());
        else if (type == 2) tree.add(left, right, input.read_uniform<13, int64_t>());
        else output.writeln_i64(tree.sum(left, right));
    }
}
