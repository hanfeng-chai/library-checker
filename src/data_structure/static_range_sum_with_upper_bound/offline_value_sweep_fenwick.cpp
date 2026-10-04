#include <toy/ds.hpp>
#include <toy/io.hpp>
#include <toy/sort.hpp>

namespace {
struct Aggregate {
    toy::u64 sum = 0;
    int count = 0;

    Aggregate &operator+=(const Aggregate &other) {
        sum += other.sum;
        count += other.count;
        return *this;
    }

    friend Aggregate operator-(Aggregate left, const Aggregate &right) {
        left.sum -= right.sum;
        left.count -= right.count;
        return left;
    }
};

struct Item {
    toy::u32 value;
    int position;
};

struct Query {
    int left;
    int right;
    toy::u32 bound;
    int index;
};
} // namespace

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<Item> items(n);
    for (int i = 0; i < n; ++i) items[i] = {input.read_uniform<10, toy::u32>(), i};
    std::vector<Query> queries(query_count);
    for (int i = 0; i < query_count; ++i) {
        queries[i].left = input.read_uniform<6, toy::u32>();
        queries[i].right = input.read_uniform<6, toy::u32>();
        queries[i].bound = input.read_uniform<10, toy::u32>();
        queries[i].index = i;
    }
    toy::radix_sort_u32<30, 15>(items.begin(), items.end(),
                                [](const Item &item) { return item.value; });
    toy::radix_sort_u32<30, 15>(queries.begin(), queries.end(),
                                [](const Query &query) { return query.bound; });

    toy::FenwickTree<Aggregate> tree(n);
    std::vector<Aggregate> answers(query_count);
    int inserted = 0;
    for (const Query &query : queries) {
        while (inserted < n && items[inserted].value <= query.bound) {
            tree.add(items[inserted].position, {items[inserted].value, 1});
            ++inserted;
        }
        answers[query.index] = tree.sum(query.left, query.right);
    }
    for (const Aggregate &answer : answers) {
        output.write_token((toy::u64)answer.count);
        output.write_token(answer.sum);
        output.put('\n');
    }
}
