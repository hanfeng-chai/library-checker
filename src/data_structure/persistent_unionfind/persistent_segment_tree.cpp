#include <toy/io.hpp>
#include <toy/persistent.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    toy::PersistentUnionFind dsu(n, query_count);
    std::vector<int> versions(query_count + 1);
    for (int query = 0; query < query_count; ++query) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int base = input.read_uniform<6, int>() + 1;
        int first = input.read_uniform<6, toy::u32>();
        int second = input.read_uniform<6, toy::u32>();
        if (type == 0)
            versions[query + 1] = dsu.unite(versions[base], first, second);
        else {
            versions[query + 1] = versions[base];
            output.write_padded_u32(
                dsu.same(versions[base], first, second));
        }
    }
}
