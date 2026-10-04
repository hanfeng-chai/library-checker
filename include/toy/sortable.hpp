#pragma once

#include <bits/extc++.h>
#include <toy/ds.hpp>
#include <toy/range.hpp>

namespace toy {

template <class T, class Operation>
class SortableSegmentTree {
    static constexpr int pool_capacity = 4'000'000;
    struct Node {
        T forward;
        T backward;
        int count;
        int left;
        int right;
    };

    int length;
    int key_limit;
    T identity;
    Operation operation;
    std::vector<Node> pool;
    PredecessorSet boundaries;
    std::vector<bool> reversed;
    std::vector<int> roots;
    std::unique_ptr<SegmentTree<T, Operation>> aggregate;

    int make_node(T value) {
        pool.push_back({value, value, 1, 0, 0});
        return pool.size() - 1;
    }

    int make_singleton(int key, T value) {
        int left = 0;
        int right = key_limit;
        std::array<bool, 32> direction;
        int depth = 0;
        while (right - left > 1) {
            int middle = (left + right) / 2;
            if (key < middle) {
                direction[depth++] = false;
                right = middle;
            } else {
                direction[depth++] = true;
                left = middle;
            }
        }
        int node = make_node(value);
        while (depth--) {
            int parent = make_node(value);
            if (direction[depth])
                pool[parent].right = node;
            else
                pool[parent].left = node;
            node = parent;
        }
        return node;
    }

    void pull(int index) {
        Node &node = pool[index];
        if (!node.left && !node.right) return;
        if (!node.left) {
            node.forward = pool[node.right].forward;
            node.backward = pool[node.right].backward;
            node.count = pool[node.right].count;
        } else if (!node.right) {
            node.forward = pool[node.left].forward;
            node.backward = pool[node.left].backward;
            node.count = pool[node.left].count;
        } else {
            node.forward = operation(pool[node.left].forward, pool[node.right].forward);
            node.backward = operation(pool[node.right].backward, pool[node.left].backward);
            node.count = pool[node.left].count + pool[node.right].count;
        }
    }

    void insert_key(int node, int left, int right, int key, T value) {
        if (right - left == 1) {
            pool[node].forward = pool[node].backward = value;
            return;
        }
        int middle = (left + right) / 2;
        if (key < middle) {
            if (!pool[node].left) pool[node].left = make_node(identity);
            insert_key(pool[node].left, left, middle, key, value);
        } else {
            if (!pool[node].right) pool[node].right = make_node(identity);
            insert_key(pool[node].right, middle, right, key, value);
        }
        pull(node);
    }

    std::pair<int, int> split(int node, int count) {
        if (!count) return {0, node};
        if (count == pool[node].count) return {node, 0};
        int left_count = pool[node].left ? pool[pool[node].left].count : 0;
        int suffix = make_node(identity);
        if (count <= left_count) {
            auto [first, second] = split(pool[node].left, count);
            pool[suffix].left = second;
            pool[suffix].right = pool[node].right;
            pool[node].left = first;
            pool[node].right = 0;
        } else {
            auto [first, second] = split(pool[node].right, count - left_count);
            pool[node].right = first;
            pool[suffix].right = second;
        }
        pull(node);
        pull(suffix);
        return {node, suffix};
    }

    int merge(int first, int second) {
        if (!first) return second;
        if (!second) return first;
        pool[first].left = merge(pool[first].left, pool[second].left);
        pool[first].right = merge(pool[first].right, pool[second].right);
        pull(first);
        return first;
    }

    T block_aggregate(int begin) const {
        return reversed[begin] ? pool[roots[begin]].backward : pool[roots[begin]].forward;
    }

    void split_at(int index) {
        if (boundaries.contains(index)) return;
        int end = boundaries.next(index);
        int begin = boundaries.previous(index);
        boundaries.insert(index);
        if (!reversed[begin]) {
            auto [first, second] = split(roots[begin], index - begin);
            roots[begin] = first;
            roots[index] = second;
            reversed[begin] = reversed[index] = false;
        } else {
            auto [first, second] = split(roots[begin], end - index);
            roots[begin] = second;
            roots[index] = first;
            reversed[begin] = reversed[index] = true;
        }
        aggregate->set(begin, block_aggregate(begin));
        aggregate->set(index, block_aggregate(index));
    }

    void initialize(const std::vector<int> &keys, const std::vector<T> &values) {
        boundaries.assign(std::string(length + 1, '1'));
        reversed.assign(length, false);
        roots.assign(length, 0);
        std::vector<T> leaves(length, identity);
        for (int i = 0; i < length; ++i) {
            roots[i] = make_singleton(keys[i], values[i]);
            leaves[i] = values[i];
        }
        aggregate = std::make_unique<SegmentTree<T, Operation>>(leaves, identity, operation);
    }

    void rebuild() {
        std::vector<int> keys;
        std::vector<T> values;
        keys.reserve(length);
        values.reserve(length);
        auto visit = [&](auto &&self, int node, int left, int right, bool reverse) -> void {
            if (!node) return;
            if (right - left == 1) {
                keys.push_back(left);
                values.push_back(pool[node].forward);
                return;
            }
            int middle = (left + right) / 2;
            if (!reverse) {
                self(self, pool[node].left, left, middle, reverse);
                self(self, pool[node].right, middle, right, reverse);
            } else {
                self(self, pool[node].right, middle, right, reverse);
                self(self, pool[node].left, left, middle, reverse);
            }
        };
        for (int begin = boundaries.next(0); begin < length; begin = boundaries.next(begin + 1))
            visit(visit, roots[begin], 0, key_limit, reversed[begin]);
        pool.clear();
        pool.push_back({});
        initialize(keys, values);
    }

    void ensure_capacity() {
        if (pool.size() > pool_capacity * 9 / 10) rebuild();
    }

  public:
    SortableSegmentTree(int key_count, const std::vector<int> &keys, const std::vector<T> &values,
                        T identity_value, Operation combine = {})
        : length(keys.size()), key_limit(key_count), identity(identity_value), operation(combine),
          boundaries(length + 1) {
        pool.reserve(pool_capacity);
        pool.push_back({});
        initialize(keys, values);
    }

    void set(int index, int key, T value) {
        ensure_capacity();
        split_at(index);
        split_at(index + 1);
        reversed[index] = false;
        roots[index] = make_singleton(key, value);
        aggregate->set(index, value);
    }
    T fold(int left, int right) {
        ensure_capacity();
        split_at(left);
        split_at(right);
        return aggregate->fold(left, right);
    }
    void sort_ascending(int left, int right) {
        ensure_capacity();
        split_at(left);
        split_at(right);
        for (int next = boundaries.next(left + 1); next != right;) {
            roots[left] = merge(roots[left], roots[next]);
            aggregate->set(next, identity);
            boundaries.erase(next);
            next = boundaries.next(next + 1);
        }
        reversed[left] = false;
        aggregate->set(left, pool[roots[left]].forward);
    }
    void sort_descending(int left, int right) {
        sort_ascending(left, right);
        reversed[left] = true;
        aggregate->set(left, pool[roots[left]].backward);
    }
};

template <class T, class Operation>
class TreapSortableSegmentTree {
    struct Node {
        T value;
        T forward;
        T backward;
        uint32_t priority;
        int key;
        int minimum_key;
        int maximum_key;
        int count;
        int left;
        int right;
    };

    int length;
    T identity;
    Operation operation;
    std::vector<Node> pool{{}};
    PredecessorSet boundaries;
    std::vector<bool> reversed;
    std::vector<int> roots;
    std::unique_ptr<SegmentTree<T, Operation>> aggregate;
    uint64_t random_state = 0x243f6a8885a308d3ULL;

    uint32_t random() {
        random_state += 0x9e3779b97f4a7c15ULL;
        uint64_t value = random_state;
        value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
        value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
        return value ^ (value >> 31);
    }

    int count(int node) const { return node ? pool[node].count : 0; }
    T forward(int node) const { return node ? pool[node].forward : identity; }
    T backward(int node) const { return node ? pool[node].backward : identity; }

    int make_node(int key, T value) {
        pool.push_back({value, value, value, random(), key, key, key, 1, 0, 0});
        return pool.size() - 1;
    }

    void pull(int index) {
        Node &node = pool[index];
        node.count = 1 + count(node.left) + count(node.right);
        node.minimum_key = node.left ? pool[node.left].minimum_key : node.key;
        node.maximum_key = node.right ? pool[node.right].maximum_key : node.key;
        node.forward = operation(operation(forward(node.left), node.value), forward(node.right));
        node.backward = operation(operation(backward(node.right), node.value), backward(node.left));
    }

    std::pair<int, int> split_rank(int node, int prefix_count) {
        if (!prefix_count) return {0, node};
        if (prefix_count == count(node)) return {node, 0};
        int left_count = count(pool[node].left);
        if (prefix_count <= left_count) {
            auto [first, second] = split_rank(pool[node].left, prefix_count);
            pool[node].left = second;
            pull(node);
            return {first, node};
        }
        auto [first, second] = split_rank(pool[node].right, prefix_count - left_count - 1);
        pool[node].right = first;
        pull(node);
        return {node, second};
    }

    std::pair<int, int> split_key(int node, int key) {
        if (!node) return {0, 0};
        if (key <= pool[node].minimum_key) return {0, node};
        if (pool[node].maximum_key < key) return {node, 0};
        if (key <= pool[node].key) {
            auto [first, second] = split_key(pool[node].left, key);
            pool[node].left = second;
            pull(node);
            return {first, node};
        }
        auto [first, second] = split_key(pool[node].right, key);
        pool[node].right = first;
        pull(node);
        return {node, second};
    }

    int meld(int first, int second) {
        if (!first) return second;
        if (!second) return first;
        if (pool[first].priority < pool[second].priority) std::swap(first, second);
        auto [left, right] = split_key(second, pool[first].key);
        pool[first].left = meld(pool[first].left, left);
        pool[first].right = meld(pool[first].right, right);
        pull(first);
        return first;
    }

    T fold_rank(int node, int left, int right, bool reverse) {
        if (left == 0 && right == pool[node].count)
            return reverse ? pool[node].backward : pool[node].forward;

        int first_child = reverse ? pool[node].right : pool[node].left;
        int second_child = reverse ? pool[node].left : pool[node].right;
        int first_count = count(first_child);
        T result = identity;
        if (left < first_count)
            result = operation(result,
                               fold_rank(first_child, left, std::min(right, first_count), reverse));
        if (left <= first_count && first_count < right)
            result = operation(result, pool[node].value);
        if (right > first_count + 1)
            result = operation(result, fold_rank(second_child, std::max(0, left - first_count - 1),
                                                 right - first_count - 1, reverse));
        return result;
    }

    T block_aggregate(int begin) const {
        return reversed[begin] ? pool[roots[begin]].backward : pool[roots[begin]].forward;
    }

    void split_at(int index) {
        if (boundaries.contains(index)) return;
        int end = boundaries.next(index);
        int begin = boundaries.previous(index);
        boundaries.insert(index);
        if (!reversed[begin]) {
            auto [first, second] = split_rank(roots[begin], index - begin);
            roots[begin] = first;
            roots[index] = second;
            reversed[begin] = reversed[index] = false;
        } else {
            auto [first, second] = split_rank(roots[begin], end - index);
            roots[begin] = second;
            roots[index] = first;
            reversed[begin] = reversed[index] = true;
        }
        aggregate->set(begin, block_aggregate(begin));
        aggregate->set(index, block_aggregate(index));
    }

    void sort_ascending_impl(int left, int right) {
        split_at(left);
        split_at(right);
        for (int next = boundaries.next(left + 1); next != right;) {
            roots[left] = meld(roots[left], roots[next]);
            aggregate->set(next, identity);
            boundaries.erase(next);
            next = boundaries.next(next + 1);
        }
        reversed[left] = false;
        aggregate->set(left, pool[roots[left]].forward);
    }

  public:
    TreapSortableSegmentTree(const std::vector<int> &keys, const std::vector<T> &values,
                             T identity_value, Operation combine = {})
        : length(keys.size()), identity(identity_value), operation(combine), boundaries(length + 1),
          reversed(length), roots(length) {
        pool.reserve(2 * length + 1);
        boundaries.assign(std::string(length + 1, '1'));
        std::vector<T> leaves(length);
        for (int i = 0; i < length; ++i) {
            roots[i] = make_node(keys[i], values[i]);
            leaves[i] = values[i];
        }
        aggregate = std::make_unique<SegmentTree<T, Operation>>(leaves, identity, operation);
    }

    void set(int index, int key, T value) {
        split_at(index);
        split_at(index + 1);
        roots[index] = make_node(key, value);
        reversed[index] = false;
        aggregate->set(index, value);
    }

    T fold(int left, int right) {
        int first = boundaries.previous(left);
        int last = boundaries.previous(right - 1);
        if (first == last)
            return fold_rank(roots[first], left - first, right - first, reversed[first]);

        T result = fold_rank(roots[first], left - first, pool[roots[first]].count, reversed[first]);
        int middle_left = boundaries.next(first + 1);
        result = operation(result, aggregate->fold(middle_left, last));
        return operation(result, fold_rank(roots[last], 0, right - last, reversed[last]));
    }

    void sort_ascending(int left, int right) { sort_ascending_impl(left, right); }

    void sort_descending(int left, int right) {
        sort_ascending_impl(left, right);
        reversed[left] = true;
        aggregate->set(left, pool[roots[left]].backward);
    }
};

template <class T, class Operation>
class PatriciaSortableSegmentTree {
    struct Node {
        T forward;
        T backward;
        uint32_t key;
        int bit;
        int count;
        int left;
        int right;
    };

    int length;
    T identity;
    Operation operation;
    std::vector<Node> pool{{}};
    std::vector<int> free_nodes;
    PredecessorSet boundaries;
    std::vector<bool> reversed;
    std::vector<int> roots;
    std::unique_ptr<SegmentTree<T, Operation>> aggregate;

    int allocate(Node node) {
        if (free_nodes.empty()) {
            pool.push_back(node);
            return pool.size() - 1;
        }
        int index = free_nodes.back();
        free_nodes.pop_back();
        pool[index] = node;
        return index;
    }

    void release(int node) { free_nodes.push_back(node); }

    int make_leaf(uint32_t key, T value) { return allocate({value, value, key, -1, 1, 0, 0}); }

    int count(int node) const { return node ? pool[node].count : 0; }

    void pull(int node) {
        Node &current = pool[node];
        current.key = pool[current.left].key;
        current.count = pool[current.left].count + pool[current.right].count;
        current.forward = operation(pool[current.left].forward, pool[current.right].forward);
        current.backward = operation(pool[current.right].backward, pool[current.left].backward);
    }

    int make_branch(int bit, int first, int second) {
        int left;
        int right;
        if (pool[first].key >> bit & 1)
            left = second, right = first;
        else
            left = first, right = second;
        int node = allocate({identity, identity, pool[left].key, bit, 0, left, right});
        pull(node);
        return node;
    }

    static int highest_bit(uint32_t value) { return std::bit_width(value) - 1; }

    int meld(int first, int second) {
        if (!first) return second;
        if (!second) return first;
        int difference = highest_bit(pool[first].key ^ pool[second].key);
        int first_bit = pool[first].bit;
        int second_bit = pool[second].bit;
        if (difference > std::max(first_bit, second_bit))
            return make_branch(difference, first, second);

        if (first_bit == second_bit) {
            pool[first].left = meld(pool[first].left, pool[second].left);
            pool[first].right = meld(pool[first].right, pool[second].right);
            release(second);
        } else if (first_bit > second_bit) {
            if (pool[second].key >> first_bit & 1)
                pool[first].right = meld(pool[first].right, second);
            else
                pool[first].left = meld(pool[first].left, second);
        } else {
            if (pool[first].key >> second_bit & 1)
                pool[second].right = meld(first, pool[second].right);
            else
                pool[second].left = meld(first, pool[second].left);
            first = second;
        }
        pull(first);
        return first;
    }

    std::pair<int, int> split_rank(int node, int prefix_count) {
        if (!prefix_count) return {0, node};
        if (prefix_count == pool[node].count) return {node, 0};
        int left_count = pool[pool[node].left].count;
        if (prefix_count == left_count) {
            int left = pool[node].left;
            int right = pool[node].right;
            release(node);
            return {left, right};
        }
        if (prefix_count < left_count) {
            auto [first, second] = split_rank(pool[node].left, prefix_count);
            pool[node].left = second;
            pull(node);
            return {first, node};
        }
        auto [first, second] = split_rank(pool[node].right, prefix_count - left_count);
        pool[node].right = first;
        pull(node);
        return {node, second};
    }

    T fold_rank(int node, int left, int right, bool reverse) {
        if (left == 0 && right == pool[node].count)
            return reverse ? pool[node].backward : pool[node].forward;
        int first_child = reverse ? pool[node].right : pool[node].left;
        int second_child = reverse ? pool[node].left : pool[node].right;
        int first_count = pool[first_child].count;
        if (right <= first_count) return fold_rank(first_child, left, right, reverse);
        if (first_count <= left)
            return fold_rank(second_child, left - first_count, right - first_count, reverse);
        return operation(fold_rank(first_child, left, first_count, reverse),
                         fold_rank(second_child, 0, right - first_count, reverse));
    }

    T block_aggregate(int begin) const {
        return reversed[begin] ? pool[roots[begin]].backward : pool[roots[begin]].forward;
    }

    void split_at(int index) {
        if (boundaries.contains(index)) return;
        int end = boundaries.next(index);
        int begin = boundaries.previous(index);
        boundaries.insert(index);
        if (!reversed[begin]) {
            auto [first, second] = split_rank(roots[begin], index - begin);
            roots[begin] = first;
            roots[index] = second;
            reversed[begin] = reversed[index] = false;
        } else {
            auto [first, second] = split_rank(roots[begin], end - index);
            roots[begin] = second;
            roots[index] = first;
            reversed[begin] = reversed[index] = true;
        }
        aggregate->set(begin, block_aggregate(begin));
        aggregate->set(index, block_aggregate(index));
    }

    void sort_ascending_impl(int left, int right) {
        split_at(left);
        split_at(right);
        for (int next = boundaries.next(left + 1); next != right;) {
            roots[left] = meld(roots[left], roots[next]);
            aggregate->set(next, identity);
            boundaries.erase(next);
            next = boundaries.next(next + 1);
        }
        reversed[left] = false;
        aggregate->set(left, pool[roots[left]].forward);
    }

  public:
    PatriciaSortableSegmentTree(const std::vector<uint32_t> &keys, const std::vector<T> &values,
                                T identity_value, Operation combine = {})
        : length(keys.size()), identity(identity_value), operation(combine), boundaries(length + 1),
          reversed(length), roots(length) {
        pool.reserve(2 * length + 1);
        free_nodes.reserve(length);
        boundaries.assign(std::string(length + 1, '1'));
        std::vector<T> leaves(length);
        for (int i = 0; i < length; ++i) {
            roots[i] = make_leaf(keys[i], values[i]);
            leaves[i] = values[i];
        }
        aggregate = std::make_unique<SegmentTree<T, Operation>>(leaves, identity, operation);
    }

    void set(int index, uint32_t key, T value) {
        split_at(index);
        split_at(index + 1);
        release(roots[index]);
        roots[index] = make_leaf(key, value);
        reversed[index] = false;
        aggregate->set(index, value);
    }

    T fold(int left, int right) {
        int first = boundaries.previous(left);
        int last = boundaries.previous(right - 1);
        if (first == last)
            return fold_rank(roots[first], left - first, right - first, reversed[first]);
        T result = fold_rank(roots[first], left - first, pool[roots[first]].count, reversed[first]);
        int middle_left = boundaries.next(first + 1);
        result = operation(result, aggregate->fold(middle_left, last));
        return operation(result, fold_rank(roots[last], 0, right - last, reversed[last]));
    }

    void sort_ascending(int left, int right) { sort_ascending_impl(left, right); }

    void sort_descending(int left, int right) {
        sort_ascending_impl(left, right);
        reversed[left] = true;
        aggregate->set(left, pool[roots[left]].backward);
    }
};

} // namespace toy
