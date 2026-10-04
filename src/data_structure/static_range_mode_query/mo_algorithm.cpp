#include <toy/io.hpp>
#include <toy/mode.hpp>

namespace {
struct Query {
    int left;
    int right;
    int index;
};
} // namespace

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> original(n), coordinates;
    for (toy::u32 &value : original) value = input.read_uniform<10, toy::u32>();
    coordinates = original;
    std::sort(coordinates.begin(), coordinates.end());
    coordinates.erase(std::unique(coordinates.begin(), coordinates.end()), coordinates.end());
    std::vector<int> values(n);
    for (int i = 0; i < n; ++i)
        values[i] = std::lower_bound(coordinates.begin(), coordinates.end(), original[i]) -
                    coordinates.begin();

    std::vector<Query> queries(query_count);
    for (int i = 0; i < query_count; ++i) {
        queries[i].left = input.read_uniform<6, toy::u32>();
        queries[i].right = input.read_uniform<6, toy::u32>();
        queries[i].index = i;
    }
    int block_size = std::max(1, n / std::max(1, (int)std::sqrt(query_count)));
    std::sort(queries.begin(), queries.end(), [&](const Query &left, const Query &right) {
        int left_block = left.left / block_size;
        int right_block = right.left / block_size;
        if (left_block != right_block) return left_block < right_block;
        return left_block & 1 ? left.right > right.right : left.right < right.right;
    });

    toy::ModeCounter counter(coordinates.size(), n);
    std::vector<std::pair<int, int>> answers(query_count);
    int left = 0;
    int right = 0;
    for (const Query &query : queries) {
        while (right < query.right) counter.add(values[right++]);
        while (query.left < left) counter.add(values[--left]);
        while (query.right < right) counter.remove(values[--right]);
        while (left < query.left) counter.remove(values[left++]);
        answers[query.index] = counter.mode();
    }
    for (auto [id, frequency] : answers) {
        output.write_token(coordinates[id]);
        output.write_token((toy::u64)frequency);
        output.put('\n');
    }
}
