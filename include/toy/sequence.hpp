#pragma once

#include <bits/extc++.h>

namespace toy {

class ReverseSumTreap {
    struct Node {
        int left = 0;
        int right = 0;
        int size = 1;
        uint32_t priority = 0;
        uint64_t value = 0;
        uint64_t sum = 0;
        bool reversed = false;
    };
    std::vector<Node> nodes{{}};
    int root = 0;
    uint64_t state = 0x243f6a8885a308d3ULL;

    uint32_t random() {
        state += 0x9e3779b97f4a7c15ULL;
        uint64_t value = state;
        value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
        value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
        return value ^ (value >> 31);
    }
    int size_of(int node) const { return node ? nodes[node].size : 0; }
    uint64_t sum_of(int node) const { return node ? nodes[node].sum : 0; }
    void reverse_node(int node) {
        if (!node) return;
        std::swap(nodes[node].left, nodes[node].right);
        nodes[node].reversed ^= true;
    }
    void push(int node) {
        if (!node || !nodes[node].reversed) return;
        reverse_node(nodes[node].left);
        reverse_node(nodes[node].right);
        nodes[node].reversed = false;
    }
    void pull(int node) {
        nodes[node].size = 1 + size_of(nodes[node].left) + size_of(nodes[node].right);
        nodes[node].sum =
            sum_of(nodes[node].left) + nodes[node].value + sum_of(nodes[node].right);
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
    explicit ReverseSumTreap(const std::vector<uint64_t>& values,
                             bool reserve_storage = true) {
        if (reserve_storage) nodes.reserve(values.size() + 1);
        for (uint64_t value : values) {
            nodes.push_back({0, 0, 1, random(), value, value, false});
            root = merge(root, nodes.size() - 1);
        }
    }
    void reverse(int left, int right) {
        auto [prefix, suffix] = split(root, right);
        auto [before, middle] = split(prefix, left);
        reverse_node(middle);
        root = merge(merge(before, middle), suffix);
    }
    uint64_t sum(int left, int right) {
        auto [prefix, suffix] = split(root, right);
        auto [before, middle] = split(prefix, left);
        uint64_t result = sum_of(middle);
        root = merge(merge(before, middle), suffix);
        return result;
    }
};

class ReverseSumSplay {
    struct Node {
        uint32_t value = 0;
        uint32_t reversed = 0;
        uint64_t sum = 0;
        int size = 0;
        int left = 0;
        int right = 0;
    };

    std::vector<Node> nodes;
    int root;

    void pull(int node) {
        nodes[node].size =
            nodes[nodes[node].left].size + 1 +
            nodes[nodes[node].right].size;
        nodes[node].sum =
            nodes[nodes[node].left].sum + nodes[node].value +
            nodes[nodes[node].right].sum;
    }

    int build(int left, int right) {
        if (left > right) return 0;
        int middle = (left + right) >> 1;
        nodes[middle].left = build(left, middle - 1);
        nodes[middle].right = build(middle + 1, right);
        pull(middle);
        return middle;
    }

    void push(int node) {
        if (!nodes[node].reversed) return;
        std::swap(nodes[node].left, nodes[node].right);
        nodes[nodes[node].left].reversed ^= 1;
        nodes[nodes[node].right].reversed ^= 1;
        nodes[node].reversed = 0;
    }

    void reverse_node(int node) {
        if (node != 0) nodes[node].reversed ^= 1;
    }

    void rotate_right(int& node) {
        int left = nodes[node].left;
        nodes[node].left = nodes[left].right;
        pull(node);
        nodes[left].right = node;
        node = left;
    }

    void rotate_left(int& node) {
        int right = nodes[node].right;
        nodes[node].right = nodes[right].left;
        pull(node);
        nodes[right].left = node;
        node = right;
    }

    void splay(int& node, int rank) {
        push(node);
        int left_size = nodes[nodes[node].left].size;
        if (rank == left_size) return;
        if (rank < left_size) {
            int& left = nodes[node].left;
            push(left);
            int left_left_size = nodes[nodes[left].left].size;
            if (rank == left_left_size) {
                rotate_right(node);
            } else if (rank < left_left_size) {
                splay(nodes[left].left, rank);
                rotate_right(node);
                rotate_right(node);
            } else {
                splay(
                    nodes[left].right,
                    rank - left_left_size - 1);
                rotate_left(left);
                rotate_right(node);
            }
        } else {
            int& right = nodes[node].right;
            rank -= left_size + 1;
            push(right);
            int right_left_size = nodes[nodes[right].left].size;
            if (rank == right_left_size) {
                rotate_left(node);
            } else if (rank < right_left_size) {
                splay(nodes[right].left, rank);
                rotate_right(right);
                rotate_left(node);
            } else {
                splay(
                    nodes[right].right,
                    rank - right_left_size - 1);
                rotate_left(node);
                rotate_left(node);
            }
        }
    }

    int isolate(int left, int right) {
        splay(root, left);
        splay(nodes[root].right, right - left);
        return nodes[nodes[root].right].left;
    }

public:
    explicit ReverseSumSplay(const std::vector<uint64_t>& values)
        : nodes(values.size() + 3) {
        for (int i = 0; i < (int)values.size(); ++i)
            nodes[i + 2].value = values[i];
        root = build(1, values.size() + 2);
    }

    void reverse(int left, int right) {
        reverse_node(isolate(left, right));
    }

    uint64_t sum(int left, int right) {
        return nodes[isolate(left, right)].sum;
    }
};

class AddMinTreap {
    struct Node {
        int left = 0, right = 0, size = 1;
        uint32_t priority = 0;
        int64_t value = 0, minimum = 0, lazy = 0;
    };
    std::vector<Node> nodes{{}};
    int root = 0;
    uint64_t state = 0x13198a2e03707344ULL;
    uint32_t random() {
        state += 0x9e3779b97f4a7c15ULL;
        uint64_t x = state;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    int size_of(int x) const { return x ? nodes[x].size : 0; }
    int64_t min_of(int x) const {
        return x ? nodes[x].minimum : std::numeric_limits<int64_t>::max();
    }
    void apply(int x, int64_t value) {
        if (!x) return;
        nodes[x].value += value;
        nodes[x].minimum += value;
        nodes[x].lazy += value;
    }
    void push(int x) {
        if (!x || !nodes[x].lazy) return;
        apply(nodes[x].left, nodes[x].lazy);
        apply(nodes[x].right, nodes[x].lazy);
        nodes[x].lazy = 0;
    }
    void pull(int x) {
        nodes[x].size = 1 + size_of(nodes[x].left) + size_of(nodes[x].right);
        nodes[x].minimum = std::min({nodes[x].value, min_of(nodes[x].left),
                                     min_of(nodes[x].right)});
    }
    std::pair<int, int> split(int x, int count) {
        if (!x) return {0, 0};
        push(x);
        if (size_of(nodes[x].left) >= count) {
            auto [a, b] = split(nodes[x].left, count);
            nodes[x].left = b; pull(x); return {a, x};
        }
        auto [a, b] = split(nodes[x].right,
                            count - size_of(nodes[x].left) - 1);
        nodes[x].right = a; pull(x); return {x, b};
    }
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (nodes[a].priority > nodes[b].priority) {
            push(a); nodes[a].right = merge(nodes[a].right, b); pull(a); return a;
        }
        push(b); nodes[b].left = merge(a, nodes[b].left); pull(b); return b;
    }
public:
    explicit AddMinTreap(const std::vector<int64_t>& values) {
        nodes.reserve(values.size() + 1);
        for (int64_t value : values) {
            nodes.push_back({0, 0, 1, random(), value, value, 0});
            root = merge(root, nodes.size() - 1);
        }
    }
    void add(int left, int right, int64_t value) {
        auto [a, c] = split(root, right); auto [p, b] = split(a, left);
        apply(b, value); root = merge(merge(p, b), c);
    }
    int64_t minimum(int left, int right) {
        auto [a, c] = split(root, right); auto [p, b] = split(a, left);
        int64_t result = min_of(b); root = merge(merge(p, b), c); return result;
    }
};

} // namespace toy
