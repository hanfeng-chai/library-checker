#include <toy/affine.hpp>
#include <toy/io.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    using Function = toy::Affine<mod>;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int queries = input.read_uniform<6, toy::u32>();
    static toy::FixedFoldableQueue<Function, toy::ComposeAffine<mod>, 500'000> queue(
        Function{}, toy::ComposeAffine<mod>{});
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0) {
            toy::u32 a = input.read_uniform<9, toy::u32>();
            toy::u32 b = input.read_uniform<9, toy::u32>();
            queue.push({a, b});
        } else if (type == 1) {
            queue.pop();
        } else {
            toy::u32 x = input.read_uniform<9, toy::u32>();
            output.write_padded_u32(queue.fold()(x));
        }
    }
}
