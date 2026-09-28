#include <toy/csr.hpp>
#include <toy/ds.hpp>
#include <toy/hash.hpp>
#include <toy/io.hpp>

namespace {
struct Query {
    int left;
    int right;
    int index;
};
}

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    toy::U64HashMap last_occurrence(n);
    std::vector<int> previous_plus_one(n);
    for (int i = 0; i < n; ++i) {
        toy::u32 value = input.read_uniform<10, toy::u32>();
        previous_plus_one[i] = last_occurrence.exchange(value, i + 1);
    }

    std::vector<Query> queries(query_count);
    for (int i = 0; i < query_count; ++i) {
        queries[i].left = input.read_uniform<6, toy::u32>();
        queries[i].right = input.read_uniform<6, toy::u32>();
        queries[i].index = i;
    }
    toy::CsrBuckets by_right(
        n + 1, std::move(queries),
        [](const Query& query) { return query.right; });

    toy::FenwickTree<int> previous_counts(n + 1);
    std::vector<int> answers(query_count);
    for (int position = 0; position < n; ++position) {
        previous_counts.add(previous_plus_one[position], 1);
        for (const Query& query : by_right[position + 1])
            answers[query.index] =
                previous_counts.prefix_sum(query.left + 1) - query.left;
    }
    for (int answer : answers) output.write_padded_u32(answer);
}
