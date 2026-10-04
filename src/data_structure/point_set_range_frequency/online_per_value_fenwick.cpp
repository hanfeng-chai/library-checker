#include <toy/io.hpp>

namespace {
struct Query {
    toy::u32 type;
    int first;
    int second;
    toy::u32 value;
};
} // namespace

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> initial(n), coordinates;
    coordinates.reserve(n + query_count);
    for (toy::u32 &value : initial) {
        value = input.read_uniform<10, toy::u32>();
        coordinates.push_back(value);
    }
    if (query_count && n == 0) input.skip_spaces();

    std::vector<Query> queries(query_count);
    for (Query &query : queries) {
        query.type = input.read_fixed<1, toy::u32>();
        query.first = input.read_uniform<6, toy::u32>();
        if (query.type == 0) {
            query.second = 0;
            query.value = input.read_uniform<10, toy::u32>();
        } else {
            query.second = input.read_uniform<6, toy::u32>();
            query.value = input.read_uniform<10, toy::u32>();
        }
        coordinates.push_back(query.value);
    }
    std::sort(coordinates.begin(), coordinates.end());
    coordinates.erase(std::unique(coordinates.begin(), coordinates.end()), coordinates.end());
    auto id_of = [&](toy::u32 value) {
        return (int)(std::lower_bound(coordinates.begin(), coordinates.end(), value) -
                     coordinates.begin());
    };

    int value_count = coordinates.size();
    std::vector<std::vector<int>> positions(value_count);
    for (int i = 0; i < n; ++i) positions[id_of(initial[i])].push_back(i);
    for (const Query &query : queries)
        if (query.type == 0) positions[id_of(query.value)].push_back(query.first);
    for (auto &value_positions : positions) {
        std::sort(value_positions.begin(), value_positions.end());
        value_positions.erase(std::unique(value_positions.begin(), value_positions.end()),
                              value_positions.end());
    }

    std::vector<std::vector<int>> trees(value_count);
    for (int value = 0; value < value_count; ++value)
        trees[value].resize(positions[value].size() + 1);
    auto add = [&](int value, int position, int delta) {
        int index = std::lower_bound(positions[value].begin(), positions[value].end(), position) -
                    positions[value].begin() + 1;
        for (; index < (int)trees[value].size(); index += index & -index)
            trees[value][index] += delta;
    };
    auto prefix_sum = [&](int value, int end) {
        int index = std::lower_bound(positions[value].begin(), positions[value].end(), end) -
                    positions[value].begin();
        int result = 0;
        for (; index; index -= index & -index) result += trees[value][index];
        return result;
    };

    std::vector<int> current(n);
    for (int i = 0; i < n; ++i) {
        current[i] = id_of(initial[i]);
        add(current[i], i, 1);
    }
    for (const Query &query : queries) {
        int value = id_of(query.value);
        if (query.type == 0) {
            add(current[query.first], query.first, -1);
            current[query.first] = value;
            add(value, query.first, 1);
        } else {
            output.write_padded_u32(prefix_sum(value, query.second) -
                                    prefix_sum(value, query.first));
        }
    }
}
