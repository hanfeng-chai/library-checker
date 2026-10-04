#pragma once

#include <bits/extc++.h>

namespace toy {

template <class T>
class PrefixSum {
    std::vector<T> prefix;

  public:
    template <class Range>
    explicit PrefixSum(const Range &values) : prefix(values.size() + 1) {
        std::partial_sum(values.begin(), values.end(), prefix.begin() + 1);
    }

    T sum(int left, int right) const { return prefix[right] - prefix[left]; }
};

template <class T, class Operation>
class SegmentTree {
    int size;
    T identity;
    Operation operation;
    std::vector<T> data;

  public:
    template <class Range>
    SegmentTree(const Range &values, T identity_value, Operation combine = {})
        : size(std::bit_ceil((unsigned)values.size())), identity(identity_value),
          operation(combine), data(2 * size, identity_value) {
        std::copy(values.begin(), values.end(), data.begin() + size);
        for (int i = size - 1; i; --i) data[i] = operation(data[2 * i], data[2 * i + 1]);
    }

    void set(int index, T value) {
        data[index += size] = value;
        while (index >>= 1) data[index] = operation(data[2 * index], data[2 * index + 1]);
    }

    T get(int index) const { return data[size + index]; }

    T fold(int left, int right) const {
        T lhs = identity, rhs = identity;
        for (left += size, right += size; left < right; left >>= 1, right >>= 1) {
            if (left & 1) lhs = operation(lhs, data[left++]);
            if (right & 1) rhs = operation(data[--right], rhs);
        }
        return operation(lhs, rhs);
    }
};

template <class T, class Operation>
class SparseTable {
    Operation operation;
    std::vector<std::vector<T>> table;

  public:
    template <class Range>
    explicit SparseTable(const Range &values, Operation combine = {})
        : operation(combine), table(std::bit_width(values.size())) {
        table[0].assign(values.begin(), values.end());
        for (int level = 1; level < (int)table.size(); ++level) {
            int half = 1 << (level - 1);
            int count = values.size() - (1 << level) + 1;
            table[level].resize(std::max(count, 0));
            for (int i = 0; i < count; ++i)
                table[level][i] = operation(table[level - 1][i], table[level - 1][i + half]);
        }
    }

    T fold_idempotent(int left, int right) const {
        int level = std::bit_width((unsigned)(right - left)) - 1;
        return operation(table[level][left], table[level][right - (1 << level)]);
    }
};

class RangeAddMinTree {
    static constexpr int64_t infinity = 4'000'000'000'000'000'000LL;

    struct PrefixMinimum {
        int64_t sum = 0;
        int64_t minimum = infinity;
    };

    int length;
    int size;
    std::vector<int64_t> difference;
    std::vector<PrefixMinimum> data;

    static PrefixMinimum combine(const PrefixMinimum &left, const PrefixMinimum &right) {
        return {left.sum + right.sum, std::min(left.minimum, left.sum + right.minimum)};
    }

    void set(int index) {
        int node = size + index;
        data[node] = {difference[index], difference[index]};
        while (node >>= 1) data[node] = combine(data[node * 2], data[node * 2 + 1]);
    }

    PrefixMinimum fold(int left, int right) const {
        uint32_t lower = left + size - 1;
        uint32_t upper = right + size;
        int width = std::bit_width(lower ^ upper) - 1;
        uint32_t mask = (uint32_t(1) << width) - 1;
        PrefixMinimum result;

        uint32_t bits = ~lower & mask;
        while (bits != 0) {
            int level = std::countr_zero(bits);
            bits ^= uint32_t(1) << level;
            result = combine(result, data[(lower >> level) ^ 1]);
        }
        bits = upper & mask;
        while (bits != 0) {
            int level = std::bit_width(bits) - 1;
            bits ^= uint32_t(1) << level;
            result = combine(result, data[(upper >> level) ^ 1]);
        }
        return result;
    }

    int64_t prefix_sum(int end) const {
        int64_t result = 0;
        for (uint32_t node = size + end; node > 1; node >>= 1)
            if (node & 1) result += data[node - 1].sum;
        return result;
    }

  public:
    explicit RangeAddMinTree(const std::vector<int64_t> &values)
        : length(values.size()), size(std::bit_ceil((unsigned)values.size())),
          difference(values.size()), data(2 * size) {
        int64_t previous = 0;
        for (int i = 0; i < length; ++i) {
            difference[i] = values[i] - previous;
            previous = values[i];
            data[size + i] = {difference[i], difference[i]};
        }
        for (int node = size - 1; node; --node)
            data[node] = combine(data[node * 2], data[node * 2 + 1]);
    }

    void add(int left, int right, int64_t value) {
        difference[left] += value;
        set(left);
        if (right < length) {
            difference[right] -= value;
            set(right);
        }
    }

    int64_t minimum_of(int left, int right) const {
        return prefix_sum(left) + fold(left, right).minimum;
    }
};

class RecursiveRangeClampAddSumTree {
    static constexpr int64_t infinity = std::numeric_limits<int64_t>::max();
    struct Node {
        int64_t maximum = -infinity, second_maximum = -infinity;
        int64_t minimum = infinity, second_minimum = infinity;
        int64_t sum = 0, lazy_add = 0;
        int maximum_count = 0, minimum_count = 0;
    };

    int size;
    std::vector<Node> data;

    void pull(int node) {
        const Node &left = data[node * 2];
        const Node &right = data[node * 2 + 1];
        Node &current = data[node];
        current.sum = left.sum + right.sum;
        current.maximum = std::max(left.maximum, right.maximum);
        current.maximum_count = (left.maximum == current.maximum ? left.maximum_count : 0) +
                                (right.maximum == current.maximum ? right.maximum_count : 0);
        current.second_maximum =
            std::max(left.maximum == current.maximum ? left.second_maximum : left.maximum,
                     right.maximum == current.maximum ? right.second_maximum : right.maximum);
        current.minimum = std::min(left.minimum, right.minimum);
        current.minimum_count = (left.minimum == current.minimum ? left.minimum_count : 0) +
                                (right.minimum == current.minimum ? right.minimum_count : 0);
        current.second_minimum =
            std::min(left.minimum == current.minimum ? left.second_minimum : left.minimum,
                     right.minimum == current.minimum ? right.second_minimum : right.minimum);
        current.lazy_add = 0;
    }
    void apply_add(int node, int length, int64_t value) {
        Node &current = data[node];
        current.sum += value * length;
        current.maximum += value;
        current.minimum += value;
        if (current.second_maximum != -infinity) current.second_maximum += value;
        if (current.second_minimum != infinity) current.second_minimum += value;
        current.lazy_add += value;
    }
    void apply_chmin(int node, int64_t value) {
        Node &current = data[node];
        current.sum += (value - current.maximum) * current.maximum_count;
        if (current.minimum == current.maximum)
            current.minimum = value;
        else if (current.second_minimum == current.maximum)
            current.second_minimum = value;
        current.maximum = value;
    }
    void apply_chmax(int node, int64_t value) {
        Node &current = data[node];
        current.sum += (value - current.minimum) * current.minimum_count;
        if (current.maximum == current.minimum)
            current.maximum = value;
        else if (current.second_maximum == current.minimum)
            current.second_maximum = value;
        current.minimum = value;
    }
    void push(int node, int left_length, int right_length) {
        Node &current = data[node];
        if (current.lazy_add) {
            apply_add(node * 2, left_length, current.lazy_add);
            apply_add(node * 2 + 1, right_length, current.lazy_add);
            current.lazy_add = 0;
        }
        if (data[node * 2].maximum > current.maximum) apply_chmin(node * 2, current.maximum);
        if (data[node * 2 + 1].maximum > current.maximum)
            apply_chmin(node * 2 + 1, current.maximum);
        if (data[node * 2].minimum < current.minimum) apply_chmax(node * 2, current.minimum);
        if (data[node * 2 + 1].minimum < current.minimum)
            apply_chmax(node * 2 + 1, current.minimum);
    }
    void build(int node, int left, int right, const std::vector<int64_t> &values) {
        if (right - left == 1) {
            int64_t value = values[left];
            data[node] = {value, -infinity, value, infinity, value, 0, 1, 1};
            return;
        }
        int middle = (left + right) / 2;
        build(node * 2, left, middle, values);
        build(node * 2 + 1, middle, right, values);
        pull(node);
    }
    void chmin(int node, int left, int right, int query_left, int query_right, int64_t value) {
        if (query_right <= left || right <= query_left || data[node].maximum <= value) return;
        if (query_left <= left && right <= query_right && data[node].second_maximum < value) {
            apply_chmin(node, value);
            return;
        }
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        chmin(node * 2, left, middle, query_left, query_right, value);
        chmin(node * 2 + 1, middle, right, query_left, query_right, value);
        pull(node);
    }
    void chmax(int node, int left, int right, int query_left, int query_right, int64_t value) {
        if (query_right <= left || right <= query_left || value <= data[node].minimum) return;
        if (query_left <= left && right <= query_right && value < data[node].second_minimum) {
            apply_chmax(node, value);
            return;
        }
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        chmax(node * 2, left, middle, query_left, query_right, value);
        chmax(node * 2 + 1, middle, right, query_left, query_right, value);
        pull(node);
    }
    void add(int node, int left, int right, int query_left, int query_right, int64_t value) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            apply_add(node, right - left, value);
            return;
        }
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        add(node * 2, left, middle, query_left, query_right, value);
        add(node * 2 + 1, middle, right, query_left, query_right, value);
        pull(node);
    }
    int64_t sum(int node, int left, int right, int query_left, int query_right) {
        if (query_right <= left || right <= query_left) return 0;
        if (query_left <= left && right <= query_right) return data[node].sum;
        int middle = (left + right) / 2;
        push(node, middle - left, right - middle);
        return sum(node * 2, left, middle, query_left, query_right) +
               sum(node * 2 + 1, middle, right, query_left, query_right);
    }

  public:
    explicit RecursiveRangeClampAddSumTree(const std::vector<int64_t> &values)
        : size(values.size()), data(values.size() * 4) {
        build(1, 0, size, values);
    }
    void chmin(int left, int right, int64_t value) { chmin(1, 0, size, left, right, value); }
    void chmax(int left, int right, int64_t value) { chmax(1, 0, size, left, right, value); }
    void add(int left, int right, int64_t value) { add(1, 0, size, left, right, value); }
    int64_t sum(int left, int right) { return sum(1, 0, size, left, right); }
};

class RangeClampAddSumTree {
    static constexpr int64_t infinity = std::numeric_limits<int64_t>::max();

    struct Node {
        int64_t maximum = -infinity;
        int64_t second_maximum = -infinity;
        int64_t minimum = infinity;
        int64_t second_minimum = infinity;
        int64_t sum = 0;
        int64_t lazy_add = 0;
        uint32_t maximum_count = 0;
        uint32_t minimum_count = 0;
    };

    int length;
    uint32_t capacity;
    uint32_t depth;
    std::vector<Node> data;

    void pull(uint32_t node) {
        const Node &left = data[node * 2];
        const Node &right = data[node * 2 + 1];
        Node &current = data[node];
        current.sum = left.sum + right.sum;
        if (left.maximum == right.maximum) {
            current.maximum = left.maximum;
            current.second_maximum = std::max(left.second_maximum, right.second_maximum);
            current.maximum_count = left.maximum_count + right.maximum_count;
        } else if (left.maximum > right.maximum) {
            current.maximum = left.maximum;
            current.second_maximum = std::max(left.second_maximum, right.maximum);
            current.maximum_count = left.maximum_count;
        } else {
            current.maximum = right.maximum;
            current.second_maximum = std::max(left.maximum, right.second_maximum);
            current.maximum_count = right.maximum_count;
        }
        if (left.minimum == right.minimum) {
            current.minimum = left.minimum;
            current.second_minimum = std::min(left.second_minimum, right.second_minimum);
            current.minimum_count = left.minimum_count + right.minimum_count;
        } else if (left.minimum < right.minimum) {
            current.minimum = left.minimum;
            current.second_minimum = std::min(left.second_minimum, right.minimum);
            current.minimum_count = left.minimum_count;
        } else {
            current.minimum = right.minimum;
            current.second_minimum = std::min(left.minimum, right.second_minimum);
            current.minimum_count = right.minimum_count;
        }
        current.lazy_add = 0;
    }

    void apply_add(uint32_t node, uint32_t node_length, int64_t value) {
        Node &current = data[node];
        current.sum += value * node_length;
        current.maximum += value;
        current.minimum += value;
        if (current.second_maximum != -infinity) current.second_maximum += value;
        if (current.second_minimum != infinity) current.second_minimum += value;
        current.lazy_add += value;
    }

    void apply_chmin(uint32_t node, int64_t value) {
        Node &current = data[node];
        current.sum += (value - current.maximum) * current.maximum_count;
        if (current.minimum == current.maximum)
            current.minimum = value;
        else if (current.second_minimum == current.maximum)
            current.second_minimum = value;
        current.maximum = value;
    }

    void apply_chmax(uint32_t node, int64_t value) {
        Node &current = data[node];
        current.sum += (value - current.minimum) * current.minimum_count;
        if (current.maximum == current.minimum)
            current.maximum = value;
        else if (current.second_maximum == current.minimum)
            current.second_maximum = value;
        current.minimum = value;
    }

    void push(uint32_t node, uint32_t node_length) {
        Node &current = data[node];
        uint32_t child_length = node_length >> 1;
        if (current.lazy_add != 0) {
            apply_add(node * 2, child_length, current.lazy_add);
            apply_add(node * 2 + 1, child_length, current.lazy_add);
            current.lazy_add = 0;
        }
        if (data[node * 2].maximum > current.maximum) apply_chmin(node * 2, current.maximum);
        if (data[node * 2].minimum < current.minimum) apply_chmax(node * 2, current.minimum);
        if (data[node * 2 + 1].maximum > current.maximum)
            apply_chmin(node * 2 + 1, current.maximum);
        if (data[node * 2 + 1].minimum < current.minimum)
            apply_chmax(node * 2 + 1, current.minimum);
    }

    void apply_chmin_subtree(uint32_t node, uint32_t node_length, int64_t value) {
        if (data[node].maximum <= value) return;
        if (data[node].second_maximum < value) {
            apply_chmin(node, value);
            return;
        }
        push(node, node_length);
        apply_chmin_subtree(node * 2, node_length >> 1, value);
        apply_chmin_subtree(node * 2 + 1, node_length >> 1, value);
        pull(node);
    }

    void apply_chmax_subtree(uint32_t node, uint32_t node_length, int64_t value) {
        if (data[node].minimum >= value) return;
        if (value < data[node].second_minimum) {
            apply_chmax(node, value);
            return;
        }
        push(node, node_length);
        apply_chmax_subtree(node * 2, node_length >> 1, value);
        apply_chmax_subtree(node * 2 + 1, node_length >> 1, value);
        pull(node);
    }

    template <class Apply>
    void apply_range(int left, int right, Apply apply) {
        uint32_t lower = left + capacity;
        uint32_t upper = right - 1 + capacity;
        if (lower == upper) {
            uint32_t node_length = capacity;
            for (uint32_t level = depth; level != 0; --level) {
                push(lower >> level, node_length);
                node_length >>= 1;
            }
            apply(lower, 1);
            while (lower >>= 1) pull(lower);
            return;
        }

        uint32_t split_level = std::bit_width(lower ^ upper) - 1;
        uint32_t node_length = capacity;
        for (uint32_t level = depth; level > split_level; --level) {
            push(lower >> level, node_length);
            node_length >>= 1;
        }
        for (uint32_t level = split_level; level != 0; --level) {
            push(lower >> level, node_length);
            push(upper >> level, node_length);
            node_length >>= 1;
        }

        apply(lower, 1);
        apply(upper, 1);
        node_length = 1;
        while ((lower >> 1) < (upper >> 1)) {
            if ((lower & 1) == 0) apply(lower + 1, node_length);
            pull(lower >>= 1);
            if (upper & 1) apply(upper - 1, node_length);
            pull(upper >>= 1);
            node_length <<= 1;
        }
        while (lower >>= 1) pull(lower);
    }

    void push_boundary_paths(uint32_t lower, uint32_t upper) {
        if (lower == upper) {
            uint32_t node_length = capacity;
            for (uint32_t level = depth; level != 0; --level) {
                push(lower >> level, node_length);
                node_length >>= 1;
            }
            return;
        }
        uint32_t split_level = std::bit_width(lower ^ upper) - 1;
        uint32_t node_length = capacity;
        for (uint32_t level = depth; level > split_level; --level) {
            push(lower >> level, node_length);
            node_length >>= 1;
        }
        for (uint32_t level = split_level; level != 0; --level) {
            push(lower >> level, node_length);
            push(upper >> level, node_length);
            node_length >>= 1;
        }
    }

  public:
    explicit RangeClampAddSumTree(const std::vector<int64_t> &values)
        : length(values.size()), capacity(std::bit_ceil((uint32_t)values.size())),
          depth(std::bit_width(capacity) - 1), data(2 * capacity) {
        for (uint32_t i = 0; i < values.size(); ++i) {
            int64_t value = values[i];
            data[capacity + i] = {value, -infinity, value, infinity, value, 0, 1, 1};
        }
        for (uint32_t node = capacity - 1; node != 0; --node) pull(node);
    }

    void chmin(int left, int right, int64_t value) {
        apply_range(left, right, [&](uint32_t node, uint32_t node_length) {
            apply_chmin_subtree(node, node_length, value);
        });
    }

    void chmax(int left, int right, int64_t value) {
        apply_range(left, right, [&](uint32_t node, uint32_t node_length) {
            apply_chmax_subtree(node, node_length, value);
        });
    }

    void add(int left, int right, int64_t value) {
        apply_range(left, right, [&](uint32_t node, uint32_t node_length) {
            apply_add(node, node_length, value);
        });
    }

    int64_t sum(int left, int right) {
        uint32_t lower = left + capacity;
        uint32_t upper = right - 1 + capacity;
        push_boundary_paths(lower, upper);
        int64_t left_sum = data[lower].sum;
        if (lower == upper) return left_sum;
        int64_t right_sum = data[upper].sum;
        while ((lower >> 1) != (upper >> 1)) {
            if ((lower & 1) == 0) left_sum += data[lower + 1].sum;
            if (upper & 1) right_sum += data[upper - 1].sum;
            lower >>= 1;
            upper >>= 1;
        }
        return left_sum + right_sum;
    }
};

} // namespace toy
