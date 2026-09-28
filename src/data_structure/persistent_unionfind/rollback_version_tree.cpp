#include <toy/io.hpp>
#include <toy/persistent.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    toy::OfflinePersistentUnionFind dsu(n, query_count);
    for (int query = 0; query < query_count; ++query) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int base = input.read_uniform<6, int>() + 1;
        int first = input.read_uniform<6, toy::u32>();
        int second = input.read_uniform<6, toy::u32>();
        if (type == 0)
            dsu.merge(base, first, second);
        else
            dsu.same(base, first, second);
    }
    for (int answer : dsu.solve()) output.write_padded_u32(answer);
}
