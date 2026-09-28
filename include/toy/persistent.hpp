#pragma once

#include <bits/extc++.h>
#include <toy/ds.hpp>

namespace toy {

template<class T>
class OfflinePersistentQueue {
    struct Operation {
        int next;
        int answer;
        T value;
    };

    std::vector<int> first_child;
    std::vector<Operation> operations;
    int answer_count = 0;

    void visit(int version, std::vector<T>& buffer, int& front, int& back,
               std::vector<T>& answers) const {
        const Operation& operation = operations[version - 1];
        if (operation.answer < 0)
            buffer[back++] = operation.value;
        else
            answers[operation.answer] = buffer[front++];

        for (int child = first_child[version]; child;
             child = operations[child - 1].next)
            visit(child, buffer, front, back, answers);

        if (operation.answer < 0)
            --back;
        else
            buffer[--front] = answers[operation.answer];
    }

public:
    explicit OfflinePersistentQueue(int operation_count)
        : first_child(operation_count + 1) {
        operations.reserve(operation_count);
    }

    void push(int base_version, T value) {
        int version = operations.size() + 1;
        operations.push_back({first_child[base_version], -1, value});
        first_child[base_version] = version;
    }

    void pop(int base_version) {
        int version = operations.size() + 1;
        operations.push_back(
            {first_child[base_version], answer_count++, T{}});
        first_child[base_version] = version;
    }

    std::vector<T> solve() const {
        std::vector<T> buffer(operations.size());
        std::vector<T> answers(answer_count);
        int front = 0;
        int back = 0;
        for (int version = first_child[0]; version;
             version = operations[version - 1].next)
            visit(version, buffer, front, back, answers);
        return answers;
    }
};

template<class T, unsigned Levels>
class PersistentQueue {
    struct Node {
        T value{};
        std::array<int, Levels> ancestor{};
    };

public:
    struct Version {
        int back = 0;
        int size = 0;
    };

private:
    std::vector<Node> nodes{{}};

    int ancestor(int node, int distance) const {
        for (unsigned level = 0; distance; ++level, distance >>= 1)
            if (distance & 1) node = nodes[node].ancestor[level];
        return node;
    }

public:
    explicit PersistentQueue(std::size_t capacity = 0) {
        nodes.reserve(capacity + 1);
    }

    Version push(Version version, T value) {
        Node node;
        node.value = value;
        node.ancestor[0] = version.back;
        for (unsigned level = 1; level < Levels; ++level)
            node.ancestor[level] =
                nodes[node.ancestor[level - 1]].ancestor[level - 1];
        nodes.push_back(node);
        return {(int)nodes.size() - 1, version.size + 1};
    }

    std::pair<Version, T> pop(Version version) const {
        int front = ancestor(version.back, version.size - 1);
        return {{version.back, version.size - 1}, nodes[front].value};
    }
};

class OfflinePersistentUnionFind {
    struct Operation {
        int next;
        int first;
        int second;
        int answer;
    };

    int vertex_count;
    std::vector<int> first_child;
    std::vector<Operation> operations;
    int answer_count = 0;

    void visit(int version, RollbackUnionFind& dsu,
               std::vector<int>& answers) const {
        const Operation& operation = operations[version - 1];
        if (operation.answer < 0)
            dsu.merge(operation.first, operation.second);
        else
            answers[operation.answer] =
                dsu.same(operation.first, operation.second);

        for (int child = first_child[version]; child;
             child = operations[child - 1].next)
            visit(child, dsu, answers);

        if (operation.answer < 0) dsu.undo();
    }

public:
    OfflinePersistentUnionFind(int vertices, int operation_count)
        : vertex_count(vertices), first_child(operation_count + 1) {
        operations.reserve(operation_count);
    }

    void merge(int base_version, int first, int second) {
        int version = operations.size() + 1;
        operations.push_back(
            {first_child[base_version], first, second, -1});
        first_child[base_version] = version;
    }

    void same(int base_version, int first, int second) {
        int version = operations.size() + 1;
        operations.push_back(
            {first_child[base_version], first, second, answer_count++});
        first_child[base_version] = version;
    }

    std::vector<int> solve() const {
        RollbackUnionFind dsu(vertex_count, operations.size());
        std::vector<int> answers(answer_count);
        for (int version = first_child[0]; version;
             version = operations[version - 1].next)
            visit(version, dsu, answers);
        return answers;
    }
};

class PersistentUnionFind {
    struct Node {
        int left = 0;
        int right = 0;
        int value = -1;
    };

    int size;
    std::vector<Node> nodes{{}};

    int get(int root, int left, int right, int index) const {
        if (!root) return -1;
        if (right - left == 1) return nodes[root].value;
        int middle = (left + right) / 2;
        return index < middle ? get(nodes[root].left, left, middle, index)
                              : get(nodes[root].right, middle, right, index);
    }

    int set(int root, int left, int right, int index, int value) {
        int copy = nodes.size();
        nodes.push_back(root ? nodes[root] : Node{});
        if (right - left == 1) {
            nodes[copy].value = value;
            return copy;
        }
        int middle = (left + right) / 2;
        if (index < middle)
            nodes[copy].left = set(nodes[copy].left, left, middle, index, value);
        else
            nodes[copy].right = set(nodes[copy].right, middle, right, index, value);
        return copy;
    }

public:
    explicit PersistentUnionFind(int n, std::size_t updates = 0)
        : size(std::bit_ceil((unsigned)n)) {
        nodes.reserve(1 + updates * 2 * std::bit_width((unsigned)size));
    }

    int leader(int root, int vertex) const {
        int parent;
        while ((parent = get(root, 0, size, vertex)) >= 0) vertex = parent;
        return vertex;
    }

    bool same(int root, int first, int second) const {
        return leader(root, first) == leader(root, second);
    }

    int unite(int root, int first, int second) {
        first = leader(root, first);
        second = leader(root, second);
        if (first == second) return root;
        int first_size = -get(root, 0, size, first);
        int second_size = -get(root, 0, size, second);
        if (first_size < second_size) {
            std::swap(first, second);
            std::swap(first_size, second_size);
        }
        root = set(root, 0, size, first, -(first_size + second_size));
        return set(root, 0, size, second, first);
    }
};

template<uint32_t Mod>
class PersistentAffineArray {
    struct Node {
        int left = 0;
        int right = 0;
        uint32_t sum = 0;
        uint32_t a = 1;
        uint32_t b = 0;
    };

    int size;
    std::vector<Node> nodes{{}};

    static uint32_t add(uint32_t first, uint32_t second) {
        uint32_t result = first + second;
        return result >= Mod ? result - Mod : result;
    }

    int make_node(Node node) {
        nodes.push_back(node);
        return nodes.size() - 1;
    }

    int build(const std::vector<uint32_t>& values, int left, int right) {
        if (right - left == 1)
            return make_node({.sum = values[left]});
        int middle = (left + right) / 2;
        int left_child = build(values, left, middle);
        int right_child = build(values, middle, right);
        return make_node(
            {left_child, right_child,
             add(nodes[left_child].sum, nodes[right_child].sum)});
    }

    int transform(int old, int length, uint32_t a, uint32_t b) {
        if (a == 1 && b == 0) return old;
        Node node = nodes[old];
        node.sum =
            ((uint64_t)a * node.sum + (uint64_t)b * length) % Mod;
        node.a = (uint64_t)a * node.a % Mod;
        node.b = ((uint64_t)a * node.b + b) % Mod;
        return make_node(node);
    }

    int merge(int left, int right) {
        return make_node(
            {left, right, add(nodes[left].sum, nodes[right].sum)});
    }

    int range_apply(int old, int left, int right, int query_left,
                    int query_right, uint32_t after_a, uint32_t after_b,
                    uint32_t update_a, uint32_t update_b) {
        int length = right - left;
        if (query_right <= left || right <= query_left)
            return transform(old, length, after_a, after_b);
        if (query_left <= left && right <= query_right) {
            uint32_t combined_a =
                (uint64_t)update_a * after_a % Mod;
            uint32_t combined_b =
                ((uint64_t)update_a * after_b + update_b) % Mod;
            return transform(old, length, combined_a, combined_b);
        }

        Node node = nodes[old];
        uint32_t next_a = (uint64_t)after_a * node.a % Mod;
        uint32_t next_b =
            ((uint64_t)after_a * node.b + after_b) % Mod;
        int middle = (left + right) / 2;
        int left_child = range_apply(
            node.left, left, middle, query_left, query_right,
            next_a, next_b, update_a, update_b);
        int right_child = range_apply(
            node.right, middle, right, query_left, query_right,
            next_a, next_b, update_a, update_b);
        return merge(left_child, right_child);
    }

    int range_copy(int destination, int source, int left, int right,
                   int query_left, int query_right,
                   uint32_t destination_after_a,
                   uint32_t destination_after_b,
                   uint32_t source_after_a,
                   uint32_t source_after_b) {
        int length = right - left;
        if (query_right <= left || right <= query_left)
            return transform(
                destination, length,
                destination_after_a, destination_after_b);
        if (query_left <= left && right <= query_right)
            return transform(
                source, length, source_after_a, source_after_b);

        Node destination_node = nodes[destination];
        Node source_node = nodes[source];
        uint32_t next_destination_a =
            (uint64_t)destination_after_a * destination_node.a % Mod;
        uint32_t next_destination_b =
            ((uint64_t)destination_after_a * destination_node.b +
             destination_after_b) %
            Mod;
        uint32_t next_source_a =
            (uint64_t)source_after_a * source_node.a % Mod;
        uint32_t next_source_b =
            ((uint64_t)source_after_a * source_node.b +
             source_after_b) %
            Mod;
        int middle = (left + right) / 2;
        int left_child = range_copy(
            destination_node.left, source_node.left, left, middle,
            query_left, query_right,
            next_destination_a, next_destination_b,
            next_source_a, next_source_b);
        int right_child = range_copy(
            destination_node.right, source_node.right, middle, right,
            query_left, query_right,
            next_destination_a, next_destination_b,
            next_source_a, next_source_b);
        return merge(left_child, right_child);
    }

    uint32_t fold(int node, int left, int right, int query_left,
                  int query_right, uint32_t after_a,
                  uint32_t after_b) const {
        if (query_left <= left && right <= query_right)
            return ((uint64_t)after_a * nodes[node].sum +
                    (uint64_t)after_b * (right - left)) %
                   Mod;

        const Node& current = nodes[node];
        uint32_t next_a = (uint64_t)after_a * current.a % Mod;
        uint32_t next_b =
            ((uint64_t)after_a * current.b + after_b) % Mod;
        int middle = (left + right) / 2;
        if (query_right <= middle)
            return fold(
                current.left, left, middle, query_left, query_right,
                next_a, next_b);
        if (middle <= query_left)
            return fold(
                current.right, middle, right, query_left, query_right,
                next_a, next_b);
        return add(
            fold(current.left, left, middle, query_left, query_right,
                 next_a, next_b),
            fold(current.right, middle, right, query_left, query_right,
                 next_a, next_b));
    }

public:
    explicit PersistentAffineArray(
        const std::vector<uint32_t>& values,
        std::size_t reserve_nodes = 0)
        : size(values.size()) {
        nodes.reserve(std::max<std::size_t>(
            2 * values.size() + 1, reserve_nodes));
        root = build(values, 0, size);
    }

    int root;

    int apply(int version, int left, int right,
              uint32_t a, uint32_t b) {
        return range_apply(
            version, 0, size, left, right, 1, 0, a, b);
    }

    int copy(int destination, int source, int left, int right) {
        return range_copy(
            destination, source, 0, size, left, right,
            1, 0, 1, 0);
    }

    uint32_t fold(int version, int left, int right) const {
        return fold(version, 0, size, left, right, 1, 0);
    }
};

} // namespace toy
