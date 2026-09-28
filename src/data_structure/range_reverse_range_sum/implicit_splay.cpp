#include <toy/io.hpp>
#include <toy/sequence.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, int>();
    int queries = input.read_uniform<6, int>();
    std::vector<toy::u64> values(n);
    for (auto& value : values) value = input.read_uniform<10, toy::u64>();
    toy::ReverseSumSplay sequence(values);
    input.skip_spaces();
    while (queries--) {
        int type = input.read_fixed<1, int>();
        int left = input.read_uniform<6, int>();
        int right = input.read_uniform<6, int>();
        if (type == 0) sequence.reverse(left, right);
        else output.writeln(sequence.sum(left, right));
    }
}
