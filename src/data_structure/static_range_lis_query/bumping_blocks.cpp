#include <toy/io.hpp>
#include <toy/lis.hpp>

namespace {
struct Range {
    int left;
    int right;
};
} // namespace

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<int> permutation(n);
    for (int &value : permutation) value = input.read_uniform<6, toy::u32>();
    std::vector<Range> queries(query_count);
    for (Range &query : queries) {
        query.left = input.read_uniform<6, toy::u32>();
        query.right = input.read_uniform<6, toy::u32>();
    }
    for (int answer : toy::static_range_lis_permutation(permutation, queries))
        output.write_padded_u32(answer);
}
