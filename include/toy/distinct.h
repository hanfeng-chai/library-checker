#pragma once
#include <toy/hash.h>
#include <toy/prefix_tree.h>

namespace toy {
struct RangeQuery {
    u32 left, right;
};
inline Buffer<u32> range_distinct(std::span<const u32> values,
                                  std::span<const RangeQuery> queries) {
    usize n = values.size();
    HashMap<u32, u32> last(n);
    Buffer<u32> previous(n), offset(n + 2), answer(queries.size());
    for (u32 i = 0; i < n; ++i) {
        auto &p = last[values[i]];
        previous[i] = std::exchange(p, i + 1);
    }
    std::fill(offset.p, offset.p + offset.n, 0u);
    for (auto q : queries) ++offset[q.right + 1];
    for (usize i = 1; i < offset.n; ++i) offset[i] += offset[i - 1];
    Buffer<u32> cursor(n + 1), order(queries.size());
    memcpy(cursor.p, offset.p, cursor.n * 4);
    for (u32 i = 0; i < queries.size(); ++i) order[cursor[queries[i].right]++] = i;
    PrefixTree32 counts(n + 1);
    for (u32 right = 0; right <= n; ++right) {
        if (right) counts.add(previous[right - 1], 1);
        // Among positions before right, exactly left belong to the prefix
        // [0,left). The remaining previous-occurrence counts represent new values.
        for (u32 i = offset[right]; i < offset[right + 1]; ++i) {
            u32 q = order[i], left = queries[q].left;
            answer[q] = counts.prefix(left + 1) - left;
        }
    }
    return answer;
}
} // namespace toy
