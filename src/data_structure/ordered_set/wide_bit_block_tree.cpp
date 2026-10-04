#include <toy/io.hpp>
#include <toy/ordered.hpp>
#include <toy/sort.hpp>

namespace {
struct Query {
    toy::u32 type;
    toy::u32 value;
    int slot = -1;
};
} // namespace

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<std::pair<toy::u32, int>> tagged;
    tagged.reserve(n + query_count);
    int slots = 0;
    for (int i = 0; i < n; ++i) tagged.push_back({input.read_uniform<10, toy::u32>(), slots++});
    if (query_count && n == 0) input.skip_spaces();

    std::vector<Query> queries(query_count);
    for (Query &query : queries) {
        query.type = input.read_fixed<1, toy::u32>();
        query.value = input.read_uniform<10, toy::u32>();
        if (query.type != 2) {
            query.slot = slots++;
            tagged.push_back({query.value, query.slot});
        }
    }

    toy::radix_sort_u32<30, 15>(tagged.begin(), tagged.end(),
                                [](const auto &item) { return item.first; });
    std::vector<toy::u32> coordinates;
    coordinates.reserve(tagged.size());
    std::vector<int> compressed(slots);
    for (const auto &[value, slot] : tagged) {
        if (coordinates.empty() || coordinates.back() != value) coordinates.push_back(value);
        compressed[slot] = coordinates.size() - 1;
    }

    toy::WideBitBlockOrderedSet set(coordinates.size(), std::span(compressed.data(), n));
    for (const Query &query : queries) {
        if (query.type == 0) {
            set.insert(compressed[query.slot]);
        } else if (query.type == 1) {
            set.erase(compressed[query.slot]);
        } else if (query.type == 2) {
            if ((int)query.value > set.size())
                output.write("-1\n");
            else
                output.writeln((toy::u64)coordinates[set.kth(query.value - 1)]);
        } else if (query.type == 3) {
            output.writeln((toy::u64)set.rank(compressed[query.slot] + 1));
        } else if (query.type == 4) {
            int count = set.rank(compressed[query.slot] + 1);
            if (!count)
                output.write("-1\n");
            else
                output.writeln((toy::u64)coordinates[set.kth(count - 1)]);
        } else {
            int count = set.rank(compressed[query.slot]);
            if (count == set.size())
                output.write("-1\n");
            else
                output.writeln((toy::u64)coordinates[set.kth(count)]);
        }
    }
}
