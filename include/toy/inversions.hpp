#pragma once

#include <bits/extc++.h>
#include <toy/sort.hpp>

namespace toy {

class PrefixIncrementPointCounter {
    static constexpr int block_shift = 8;
    static constexpr int block_size = 1 << block_shift;

    std::vector<int> direct;
    std::vector<int> lazy;

  public:
    explicit PrefixIncrementPointCounter(int size)
        : direct(size), lazy((size + block_size - 1) / block_size) {}

    int get(int index) const { return direct[index] + lazy[index >> block_shift]; }

    void increment_prefix(int end) {
        int block = end >> block_shift;
        for (int i = 0; i < block; ++i) ++lazy[i];
        for (int i = block * block_size; i <= end; ++i) ++direct[i];
    }
};

template <class Range>
std::vector<uint64_t> static_range_inversion_counts(const std::vector<uint32_t> &input,
                                                    const std::vector<Range> &ranges) {
    struct OrderedValue {
        uint32_t value;
        int index;
    };
    struct Query {
        int left;
        int right;
        int index;
    };
    struct DeltaEvent {
        int left;
        int right;
        int sign;
        int add_length;
        int query;
    };

    int n = input.size();
    std::vector<OrderedValue> ordered(n);
    for (int i = 0; i < n; ++i) ordered[i] = {input[i], i};
    radix_sort_u32<30, 15>(ordered.begin(), ordered.end(),
                           [](const OrderedValue &item) { return item.value; });
    std::vector<int> rank(n);
    for (int i = 0; i < n; ++i) rank[ordered[i].index] = i;

    std::vector<Query> queries;
    queries.reserve(ranges.size());
    for (int i = 0; i < (int)ranges.size(); ++i)
        queries.push_back({ranges[i].left, ranges[i].right - 1, i});
    constexpr int query_block_shift = 9;
    std::sort(queries.begin(), queries.end(), [](const Query &a, const Query &b) {
        int left_block = a.left >> query_block_shift;
        int right_block = b.left >> query_block_shift;
        if (left_block != right_block) return left_block < right_block;
        return left_block & 1 ? a.right > b.right : a.right < b.right;
    });

    std::vector<std::vector<DeltaEvent>> events(n);
    int current_left = 0;
    int current_right = 0;
    auto add_event = [&](int sweep, int left, int right, int sign, int add_length, int query) {
        if (sweep >= 0) events[sweep].push_back({left, right, sign, add_length, query});
    };
    for (const Query &query : queries) {
        if (current_right < query.right) {
            add_event(current_left - 1, current_right + 1, query.right, 1, 0, query.index);
            current_right = query.right;
        }
        if (query.left < current_left) {
            add_event(current_right, query.left, current_left - 1, 1, 1, query.index);
            current_left = query.left;
        }
        if (query.right < current_right) {
            add_event(current_left - 1, query.right + 1, current_right, -1, 0, query.index);
            current_right = query.right;
        }
        if (current_left < query.left) {
            add_event(current_right, current_left, query.left - 1, -1, 1, query.index);
            current_left = query.left;
        }
    }

    PrefixIncrementPointCounter counter(n);
    std::vector<int64_t> prefix_inversions(n);
    std::vector<int64_t> delta(ranges.size());
    for (int sweep = 0; sweep < n; ++sweep) {
        prefix_inversions[sweep] = counter.get(rank[sweep]);
        counter.increment_prefix(rank[sweep]);
        for (const DeltaEvent &event : events[sweep]) {
            for (int position = event.left; position <= event.right; ++position) {
                int contribution =
                    (sweep - position + 1) * event.add_length - counter.get(rank[position]);
                delta[event.query] += (int64_t)event.sign * contribution;
            }
        }
    }
    std::partial_sum(prefix_inversions.begin(), prefix_inversions.end(), prefix_inversions.begin());

    int64_t running_delta = 0;
    std::vector<uint64_t> answers(ranges.size());
    for (const Query &query : queries) {
        running_delta += delta[query.index];
        int64_t base = prefix_inversions[query.right];
        if (query.left) base -= prefix_inversions[query.left - 1];
        answers[query.index] = running_delta + base;
    }
    return answers;
}

} // namespace toy
