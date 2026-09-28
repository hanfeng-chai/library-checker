#include <toy/hash.hpp>
#include <toy/io.hpp>

namespace {
struct Query {
    int left;
    int right;
    toy::u32 value;
};

struct Endpoint {
    int query;
    toy::u32 value;
    bool right;
};
}

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    for (toy::u32& value : values)
        value = input.read_uniform<10, toy::u32>();
    if (query_count && n == 0) input.skip_spaces();

    std::vector<Query> queries(query_count);
    std::vector<int> offsets(n + 2);
    for (Query& query : queries) {
        query.left = input.read_uniform<6, toy::u32>();
        query.right = input.read_uniform<6, toy::u32>();
        query.value = input.read_uniform<10, toy::u32>();
        ++offsets[query.left + 1];
        ++offsets[query.right + 1];
    }
    std::partial_sum(offsets.begin(), offsets.end(), offsets.begin());
    std::vector<int> cursor = offsets;
    std::vector<Endpoint> endpoints(2 * query_count);
    for (int i = 0; i < query_count; ++i) {
        const Query& query = queries[i];
        endpoints[cursor[query.left]++] = {i, query.value, false};
        endpoints[cursor[query.right]++] = {i, query.value, true};
    }

    toy::U64HashMap frequencies(n);
    std::vector<int> answers(query_count);
    for (int position = 0; position <= n; ++position) {
        for (int i = offsets[position]; i < offsets[position + 1]; ++i) {
            const Endpoint& endpoint = endpoints[i];
            int frequency = frequencies.get(endpoint.value);
            answers[endpoint.query] += endpoint.right ? frequency : -frequency;
        }
        if (position != n) frequencies.add(values[position], 1);
    }
    for (int answer : answers) output.write_padded_u32(answer);
}
