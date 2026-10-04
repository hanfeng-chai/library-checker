#include <toy/affine.hpp>
#include <toy/io.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    using Function = toy::Affine<mod>;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int queries = input.read_uniform<6, toy::u32>();
    static toy::FixedFoldableDeque<Function, toy::ComposeAffine<mod>, 500'000> deque(
        Function{}, toy::ComposeAffine<mod>{});
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type <= 1) {
            Function function{input.read_uniform<9, toy::u32>(), input.read_uniform<9, toy::u32>()};
            if (type == 0)
                deque.push_front(function);
            else
                deque.push_back(function);
        } else if (type == 2) {
            deque.pop_front();
        } else if (type == 3) {
            deque.pop_back();
        } else {
            toy::u32 x = input.read_uniform<9, toy::u32>();
            output.write_padded_u32(deque.fold()(x));
        }
    }
}
