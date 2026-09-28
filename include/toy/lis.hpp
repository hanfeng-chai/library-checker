#pragma once

#include <bits/extc++.h>
#include <toy/csr.hpp>
#include <toy/ds.hpp>

namespace toy {

class PermutationLisBumping {
    template<bool Minimum>
    class IntHeap {
        std::vector<int> values;

        static bool better(int left, int right) {
            if constexpr (Minimum) return left < right;
            else return left > right;
        }

        void sift_down() {
            int value = values.front();
            int index = 0;
            while (2 * index + 1 < (int)values.size()) {
                int child = 2 * index + 1;
                if (child + 1 < (int)values.size() &&
                    better(values[child + 1], values[child]))
                    ++child;
                if (!better(values[child], value)) break;
                values[index] = values[child];
                index = child;
            }
            values[index] = value;
        }

    public:
        void reserve(int capacity) { values.reserve(capacity); }
        bool empty() const { return values.empty(); }
        int top() const { return values.front(); }
        void clear() { values.clear(); }

        void push(int value) {
            int index = values.size();
            values.push_back(value);
            while (index) {
                int parent = (index - 1) / 2;
                if (!better(value, values[parent])) break;
                values[index] = values[parent];
                index = parent;
            }
            values[index] = value;
        }

        int pop() {
            int result = values.front();
            if (values.size() == 1) {
                values.pop_back();
                return result;
            }
            values.front() = values.back();
            values.pop_back();
            sift_down();
            return result;
        }

        int replace_top(int value) {
            int result = values.front();
            values.front() = value;
            sift_down();
            return result;
        }

        int push_pop(int value) {
            if (!better(values.front(), value)) return value;
            return replace_top(value);
        }
    };

    int length;
    int block_size;
    int block_count;
    std::vector<int> bump_positions;
    std::vector<IntHeap<false>> block_maximums;
    std::vector<IntHeap<true>> pending_replacements;

    int block_left(int block) const { return block * block_size; }
    int block_right(int block) const {
        return std::min(length, (block + 1) * block_size);
    }

    void materialize(int block) {
        auto& pending = pending_replacements[block];
        if (pending.empty()) return;
        for (int value = block_left(block); value < block_right(block); ++value) {
            if (!bump_positions[value]) continue;
            bump_positions[value] =
                pending.push_pop(bump_positions[value]);
        }
        pending.clear();
    }

    void rebuild(int block) {
        auto& maximums = block_maximums[block];
        maximums.clear();
        for (int value = block_left(block); value < block_right(block); ++value)
            if (bump_positions[value]) maximums.push(bump_positions[value]);
    }

public:
    explicit PermutationLisBumping(int n)
        : length(n),
          block_size(std::max(1, (int)std::sqrt(n))),
          block_count((n + block_size - 1) / block_size),
          bump_positions(n), block_maximums(block_count),
          pending_replacements(block_count) {
        for (int block = 0; block < block_count; ++block) {
            block_maximums[block].reserve(block_size);
            pending_replacements[block].reserve(block_size);
        }
    }

    int insert(int position, int value) {
        int block = value / block_size;
        materialize(block);
        bump_positions[value] = position + 1;
        block_maximums[block].push(position + 1);

        int evicted = 0;
        bool changed = false;
        for (int next = value + 1; next < block_right(block); ++next) {
            if (evicted < bump_positions[next]) {
                std::swap(evicted, bump_positions[next]);
                changed = true;
            }
        }
        if (changed) rebuild(block);

        for (++block; block < block_count; ++block) {
            auto& maximums = block_maximums[block];
            if (maximums.empty() || maximums.top() <= evicted) continue;
            pending_replacements[block].push(evicted);
            if (evicted) evicted = maximums.replace_top(evicted);
            else evicted = maximums.pop();
        }
        return evicted;
    }
};

template<class Range>
std::vector<int> static_range_lis_permutation(
    const std::vector<int>& permutation, const std::vector<Range>& queries) {
    struct IndexedQuery {
        int left;
        int right;
        int index;
    };

    int n = permutation.size();
    std::vector<IndexedQuery> indexed;
    indexed.reserve(queries.size());
    for (int i = 0; i < (int)queries.size(); ++i)
        indexed.push_back({queries[i].left, queries[i].right, i});
    CsrBuckets by_right(
        n + 1, std::move(indexed),
        [](const IndexedQuery& query) { return query.right; });

    PermutationLisBumping bumping(n);
    FenwickTree<int> active(n);
    std::vector<int> answers(queries.size());
    for (int position = 0; position < n; ++position) {
        active.add(position, 1);
        int evicted = bumping.insert(position, permutation[position]);
        if (evicted) active.add(evicted - 1, -1);
        for (const IndexedQuery& query : by_right[position + 1])
            answers[query.index] = active.sum(query.left, query.right);
    }
    return answers;
}

} // namespace toy
