#include <toy/io.hpp>
#include <toy/linear_range.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, int>();
    int q = input.read_uniform<6, int>();
    std::vector<int64_t> values(n);
    for (auto &value : values) value = input.read_uniform<9, int64_t>();
    toy::KineticRangeLinearAddMinTree tree(values);
    while (q--) {
        int type = input.read_fixed<1, int>();
        int left = input.read_uniform<6, int>();
        int right = input.read_uniform<6, int>();
        if (!type) {
            int64_t slope = input.read_uniform<9, int64_t>();
            int64_t intercept = input.read_uniform<9, int64_t>();
            tree.add(left, right, slope, intercept);
        } else {
            output.writeln_i64(tree.minimum(left, right));
        }
    }
}
