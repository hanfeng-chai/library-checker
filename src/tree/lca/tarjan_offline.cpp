#include <toy/io.hpp>
#include <toy/tree.hpp>

int main() {
    static toy::FixedOfflineLca<500'000, 500'000> tree;
    toy::Reader input(toy::direct_mapping);
    toy::Writer<1 << 20> output;
    int n = input.read_uniform<6, uint32_t>();
    int q = input.read_uniform<6, uint32_t>();
    tree.reset(n, q);
    for (int vertex = 1; vertex < n; ++vertex)
        tree.add_parent(vertex, input.read_uniform<6, uint32_t>());
    for (int query = 0; query < q; ++query) {
        int first = input.read_uniform<6, uint32_t>();
        int second = input.read_uniform<6, uint32_t>();
        tree.add_query(query, first, second);
    }
    const int *answer = tree.solve();
    for (int query = 0; query < q; ++query) output.write_token_u32_6(answer[query]);
}
