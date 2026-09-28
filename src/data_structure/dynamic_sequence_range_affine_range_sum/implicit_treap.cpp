#include <toy/implicit_treap.hpp>
#include <toy/io.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    for (toy::u32& value : values)
        value = input.read_uniform<9, toy::u32>();

    toy::AffineImplicitTreap<mod> sequence(values, query_count);
    while (query_count--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0) {
            int index = input.read_uniform<6, toy::u32>();
            sequence.insert(index, input.read_uniform<9, toy::u32>());
        } else if (type == 1) {
            sequence.erase(input.read_uniform<6, toy::u32>());
        } else {
            int left = input.read_uniform<6, toy::u32>();
            int right = input.read_uniform<6, toy::u32>();
            if (type == 2) {
                sequence.reverse(left, right);
            } else if (type == 3) {
                toy::u32 multiplier =
                    input.read_uniform<9, toy::u32>();
                toy::u32 addend = input.read_uniform<9, toy::u32>();
                sequence.apply(left, right, {multiplier, addend});
            } else {
                output.write_padded_u32(sequence.fold(left, right));
            }
        }
    }
}
