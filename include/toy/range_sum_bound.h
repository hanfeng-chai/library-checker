#pragma once
#include <toy/count_sum.h>
#include <toy/radix_sort.h>

namespace toy {
struct BoundedSumQuery {
    u32 left, right, bound, index;
};
inline Buffer<BoundedSum> range_sum_bound(std::span<const u32> input,
                                          Buffer<BoundedSumQuery> queries) {
    struct Item {
        u32 value, index;
    };
    Buffer<Item> values(input.size());
    for (u32 i = 0; i < input.size(); ++i) values[i] = {input[i], i};
    radix_sort(std::span(values.p, values.n), [](Item x) { return x.value; });
    radix_sort(std::span(queries.p, queries.n), [](BoundedSumQuery q) { return q.bound; });
    CountSumTree tree(input.size());
    Buffer<BoundedSum> answer(queries.n);
    usize used = 0;
    for (auto q : std::span(queries.p, queries.n)) {
        while (used < values.n && values[used].value <= q.bound) {
            if (used + 8 < values.n) tree.prefetch(values[used + 8].index);
            auto x = values[used++];
            tree.add(x.index, x.value);
        }
        answer[q.index] = tree.sum(q.left, q.right);
    }
    return answer;
}
} // namespace toy
