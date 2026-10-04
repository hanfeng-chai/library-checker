#include <toy/ds.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::CompactWriter<> output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    toy::DisjointSetUnion dsu(n);
    while (queries--) {
        int type = input.read_fixed<1, toy::u32>();
        int u = input.read_uniform<6, toy::u32>();
        int v = input.read_uniform<6, toy::u32>();
        if (type == 0)
            dsu.merge(u, v);
        else
            output.writeln_fixed<1>((toy::u64)dsu.same(u, v));
    }
}
