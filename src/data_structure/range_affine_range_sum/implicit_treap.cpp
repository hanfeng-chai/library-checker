#include <toy/implicit_treap.hpp>
#include <toy/io.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    for (auto &value : values) value = input.read_uniform<9, toy::u32>();
    toy::AffineImplicitTreap<mod> sequence(values);
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int left = input.read_uniform<6, toy::u32>();
        int right = input.read_uniform<6, toy::u32>();
        if (type == 0) {
            toy::u32 b = input.read_uniform<9, toy::u32>();
            toy::u32 c = input.read_uniform<9, toy::u32>();
            sequence.apply(left, right, {b, c});
        } else {
            output.write_padded_u32(sequence.fold(left, right));
        }
    }
}
