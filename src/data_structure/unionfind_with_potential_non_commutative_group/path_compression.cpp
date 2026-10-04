#include <toy/group.hpp>
#include <toy/io.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    using Group = toy::Matrix2Group<mod>;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    toy::PotentialUnionFind<Group> dsu(n);
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int first = input.read_uniform<6, toy::u32>();
        int second = input.read_uniform<6, toy::u32>();
        if (type == 0) {
            Group::Value value{input.read_uniform<9, toy::u32>(), input.read_uniform<9, toy::u32>(),
                               input.read_uniform<9, toy::u32>(),
                               input.read_uniform<9, toy::u32>()};
            output.write_padded_u32(dsu.unite(first, second, value));
        } else {
            auto value = dsu.difference(first, second);
            if (!value) {
                output.write(" -1\n");
            } else {
                output.write_padded_u32(value->a);
                output.write_padded_u32(value->b);
                output.write_padded_u32(value->c);
                output.write_padded_u32(value->d);
            }
        }
    }
}
