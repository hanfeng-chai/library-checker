#pragma once

#include <toy/affine.hpp>

namespace toy {

template<uint32_t Mod>
class AffineSplaySequence {
    using Function = Affine<Mod>;

    struct Node {
        int left = 0;
        int right = 0;
        int size = 1;
        uint32_t value = 0;
        uint32_t sum = 0;
        Function lazy{};
        bool reversed = false;
    };

    std::vector<Node> nodes{{.size = 0}};
    int root = 0;

    int size_of(int node) const { return nodes[node].size; }
    uint32_t sum_of(int node) const { return nodes[node].sum; }

    static uint32_t add_sum(uint32_t first, uint32_t second) {
        uint32_t result = first + second;
        return result >= Mod ? result - Mod : result;
    }

    static uint32_t replace_sum(
        uint32_t total, uint32_t old_part, uint32_t new_part) {
        total = total >= old_part ? total - old_part
                                  : total + Mod - old_part;
        return add_sum(total, new_part);
    }

    void apply_node(int node, Function function) {
        if (!node) return;
        Node& current = nodes[node];
        current.value = function(current.value);
        current.sum =
            ((uint64_t)function.a * current.sum +
             (uint64_t)function.b * current.size) %
            Mod;
        current.lazy = ComposeAffine<Mod>{}(current.lazy, function);
    }

    void reverse_node(int node) {
        if (!node) return;
        std::swap(nodes[node].left, nodes[node].right);
        nodes[node].reversed ^= true;
    }

    void push(int node) {
        if (nodes[node].reversed) {
            reverse_node(nodes[node].left);
            reverse_node(nodes[node].right);
            nodes[node].reversed = false;
        }
        Function function = nodes[node].lazy;
        if (function.a == 1 && function.b == 0) return;
        apply_node(nodes[node].left, function);
        apply_node(nodes[node].right, function);
        nodes[node].lazy = {};
    }

    void pull(int node) {
        Node& current = nodes[node];
        current.size =
            1 + size_of(current.left) + size_of(current.right);
        uint32_t sum = sum_of(current.left) + current.value;
        if (sum >= Mod) sum -= Mod;
        sum += sum_of(current.right);
        if (sum >= Mod) sum -= Mod;
        current.sum = sum;
    }

    int build(int left, int right) {
        if (left == right) return 0;
        int middle = (left + right) / 2;
        nodes[middle].left = build(left, middle);
        nodes[middle].right = build(middle + 1, right);
        pull(middle);
        return middle;
    }

    void rotate_right(int& node) {
        int old_size = nodes[node].size;
        uint32_t old_sum = nodes[node].sum;
        int child = nodes[node].left;
        nodes[node].left = nodes[child].right;
        pull(node);
        nodes[child].right = node;
        nodes[child].size = old_size;
        nodes[child].sum = old_sum;
        node = child;
    }

    void rotate_left(int& node) {
        int old_size = nodes[node].size;
        uint32_t old_sum = nodes[node].sum;
        int child = nodes[node].right;
        nodes[node].right = nodes[child].left;
        pull(node);
        nodes[child].left = node;
        nodes[child].size = old_size;
        nodes[child].sum = old_sum;
        node = child;
    }

    void splay(int& node, int rank) {
        push(node);
        int left_size = size_of(nodes[node].left);
        if (rank == left_size) return;
        if (rank < left_size) {
            int& left = nodes[node].left;
            push(left);
            int left_left_size = size_of(nodes[left].left);
            if (rank == left_left_size) {
                rotate_right(node);
            } else if (rank < left_left_size) {
                splay(nodes[left].left, rank);
                rotate_right(node);
                rotate_right(node);
            } else {
                splay(nodes[left].right, rank - left_left_size - 1);
                rotate_left(left);
                rotate_right(node);
            }
        } else {
            rank -= left_size + 1;
            int& right = nodes[node].right;
            push(right);
            int right_left_size = size_of(nodes[right].left);
            if (rank == right_left_size) {
                rotate_left(node);
            } else if (rank < right_left_size) {
                splay(nodes[right].left, rank);
                rotate_right(right);
                rotate_left(node);
            } else {
                splay(nodes[right].right, rank - right_left_size - 1);
                rotate_left(node);
                rotate_left(node);
            }
        }
    }

    int expose(int left, int right) {
        splay(root, left);
        splay(nodes[root].right, right - left);
        return nodes[nodes[root].right].left;
    }

    void replace_exposed_sum(uint32_t old_sum, uint32_t new_sum) {
        int successor = nodes[root].right;
        nodes[successor].sum =
            replace_sum(nodes[successor].sum, old_sum, new_sum);
        nodes[root].sum =
            replace_sum(nodes[root].sum, old_sum, new_sum);
    }

public:
    explicit AffineSplaySequence(
        const std::vector<uint32_t>& values,
        std::size_t extra_capacity = 0) {
        nodes.reserve(values.size() + extra_capacity + 3);
        nodes.push_back({});
        for (uint32_t value : values)
            nodes.push_back(
                {.value = value, .sum = value});
        nodes.push_back({});
        root = build(1, nodes.size());
    }

    void insert(int index, uint32_t value) {
        splay(root, index);
        splay(nodes[root].right, 0);
        int node = nodes.size();
        nodes.push_back({.value = value, .sum = value});
        int successor = nodes[root].right;
        nodes[successor].left = node;
        ++nodes[successor].size;
        ++nodes[root].size;
        nodes[successor].sum = add_sum(nodes[successor].sum, value);
        nodes[root].sum = add_sum(nodes[root].sum, value);
    }

    void erase(int index) {
        int removed = expose(index, index + 1);
        int successor = nodes[root].right;
        nodes[successor].left = 0;
        --nodes[successor].size;
        --nodes[root].size;
        replace_exposed_sum(nodes[removed].sum, 0);
    }

    void reverse(int left, int right) {
        reverse_node(expose(left, right));
    }

    void apply(int left, int right, Function function) {
        int middle = expose(left, right);
        uint32_t old_sum = nodes[middle].sum;
        apply_node(middle, function);
        replace_exposed_sum(old_sum, nodes[middle].sum);
    }

    uint32_t fold(int left, int right) {
        return sum_of(expose(left, right));
    }

    uint32_t get(int index) { return fold(index, index + 1); }
};

} // namespace toy
