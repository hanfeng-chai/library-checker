#pragma once

#include <toy/affine.hpp>

namespace toy {

template<uint32_t Mod>
class AffineImplicitTreap {
    using Function = Affine<Mod>;
    struct Node {
        int left = 0;
        int right = 0;
        int size = 1;
        uint32_t value = 0;
        uint32_t sum = 0;
        uint32_t priority = 0;
        Function lazy{};
        bool reversed = false;
    };

    std::vector<Node> nodes{{}};
    int root = 0;
    uint64_t random_state = 0x243f6a8885a308d3ULL;

    uint32_t random() {
        random_state += 0x9e3779b97f4a7c15ULL;
        uint64_t value = random_state;
        value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
        value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
        return value ^ (value >> 31);
    }

    int size_of(int node) const { return node ? nodes[node].size : 0; }
    uint32_t sum_of(int node) const { return node ? nodes[node].sum : 0; }

    void apply(int node, Function function) {
        if (!node) return;
        Node& current = nodes[node];
        current.value = function(current.value);
        current.sum = ((uint64_t)function.a * current.sum +
                       (uint64_t)function.b * current.size) % Mod;
        current.lazy = ComposeAffine<Mod>{}(current.lazy, function);
    }

    void push(int node) {
        if (!node) return;
        if (nodes[node].reversed) {
            for (int child : {nodes[node].left, nodes[node].right}) {
                if (!child) continue;
                std::swap(nodes[child].left, nodes[child].right);
                nodes[child].reversed ^= true;
            }
            nodes[node].reversed = false;
        }
        Function function = nodes[node].lazy;
        if (function.a == 1 && function.b == 0) return;
        apply(nodes[node].left, function);
        apply(nodes[node].right, function);
        nodes[node].lazy = {};
    }

    void pull(int node) {
        Node& current = nodes[node];
        current.size = 1 + size_of(current.left) + size_of(current.right);
        current.sum = (sum_of(current.left) + current.value) % Mod;
        current.sum += sum_of(current.right);
        if (current.sum >= Mod) current.sum -= Mod;
    }

    std::pair<int, int> split(int node, int count) {
        if (!node) return {0, 0};
        push(node);
        if (size_of(nodes[node].left) >= count) {
            auto [left, middle] = split(nodes[node].left, count);
            nodes[node].left = middle;
            pull(node);
            return {left, node};
        }
        auto [middle, right] =
            split(nodes[node].right, count - size_of(nodes[node].left) - 1);
        nodes[node].right = middle;
        pull(node);
        return {node, right};
    }

    int merge(int left, int right) {
        if (!left || !right) return left ? left : right;
        if (nodes[left].priority > nodes[right].priority) {
            push(left);
            nodes[left].right = merge(nodes[left].right, right);
            pull(left);
            return left;
        }
        push(right);
        nodes[right].left = merge(left, nodes[right].left);
        pull(right);
        return right;
    }

public:
    explicit AffineImplicitTreap(const std::vector<uint32_t>& values,
                                 std::size_t extra_capacity = 0) {
        nodes.reserve(values.size() + extra_capacity + 1);
        for (uint32_t value : values) {
            nodes.push_back({0, 0, 1, value, value, random(), {}, false});
            root = merge(root, nodes.size() - 1);
        }
    }

    void insert(int index, uint32_t value) {
        auto [left, right] = split(root, index);
        nodes.push_back({0, 0, 1, value, value, random(), {}, false});
        root = merge(merge(left, nodes.size() - 1), right);
    }

    void erase(int index) {
        auto [prefix, right] = split(root, index + 1);
        auto [left, removed] = split(prefix, index);
        (void)removed;
        root = merge(left, right);
    }

    void reverse(int left, int right) {
        auto [prefix, suffix] = split(root, right);
        auto [before, middle] = split(prefix, left);
        if (middle) {
            std::swap(nodes[middle].left, nodes[middle].right);
            nodes[middle].reversed ^= true;
        }
        root = merge(merge(before, middle), suffix);
    }

    void apply(int left, int right, Function function) {
        auto [prefix, suffix] = split(root, right);
        auto [before, middle] = split(prefix, left);
        apply(middle, function);
        root = merge(merge(before, middle), suffix);
    }

    uint32_t fold(int left, int right) {
        auto [prefix, suffix] = split(root, right);
        auto [before, middle] = split(prefix, left);
        uint32_t result = sum_of(middle);
        root = merge(merge(before, middle), suffix);
        return result;
    }

    uint32_t get(int index) { return fold(index, index + 1); }
};

} // namespace toy
