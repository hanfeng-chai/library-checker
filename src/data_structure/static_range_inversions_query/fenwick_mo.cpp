#include <toy/ds.hpp>
#include <toy/io.hpp>

struct Query {
    int left, right, index;
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<int> values(n);
    for (int& value : values) value = input.read_uniform<10, toy::u32>();
    std::vector<int> order = values;
    std::sort(order.begin(), order.end());
    order.erase(std::unique(order.begin(), order.end()), order.end());
    for (int& value : values)
        value = std::lower_bound(order.begin(), order.end(), value) - order.begin();
    std::vector<Query> queries(query_count);
    for (int i = 0; i < query_count; ++i) {
        queries[i].left = input.read_uniform<6, toy::u32>();
        queries[i].right = input.read_uniform<6, toy::u32>();
        queries[i].index = i;
    }
    int block = std::max(1, n / std::max(1, (int)std::sqrt(query_count)));
    std::sort(queries.begin(), queries.end(), [&](const Query& a, const Query& b) {
        int first = a.left / block, second = b.left / block;
        if (first != second) return first < second;
        return first & 1 ? a.right > b.right : a.right < b.right;
    });
    toy::FenwickTree<int> frequency((int)order.size());
    std::vector<toy::u64> answers(query_count);
    int left = 0, right = 0, size = 0;
    toy::u64 inversions = 0;
    auto add_right = [&](int value) {
        inversions += size - frequency.prefix_sum(value + 1);
        frequency.add(value, 1);
        ++size;
    };
    auto add_left = [&](int value) {
        inversions += frequency.prefix_sum(value);
        frequency.add(value, 1);
        ++size;
    };
    auto remove_right = [&](int value) {
        frequency.add(value, -1);
        --size;
        inversions -= size - frequency.prefix_sum(value + 1);
    };
    auto remove_left = [&](int value) {
        frequency.add(value, -1);
        --size;
        inversions -= frequency.prefix_sum(value);
    };
    for (Query query : queries) {
        while (right < query.right) add_right(values[right++]);
        while (query.left < left) add_left(values[--left]);
        while (query.right < right) remove_right(values[--right]);
        while (left < query.left) remove_left(values[left++]);
        answers[query.index] = inversions;
    }
    for (toy::u64 answer : answers) output.writeln(answer);
}
