#pragma once

#include <bits/extc++.h>

namespace toy {

template<uint32_t Mod>
struct AdditiveModGroup {
    using Value = uint32_t;
    static constexpr Value identity() { return 0; }
    static Value multiply(Value first, Value second) {
        uint32_t result = first + second;
        return result >= Mod ? result - Mod : result;
    }
    static Value inverse(Value value) { return value ? Mod - value : 0; }
};

template<uint32_t Mod>
struct Matrix2Group {
    struct Value {
        uint32_t a, b, c, d;
        friend bool operator==(Value, Value) = default;
    };
    static constexpr Value identity() { return {1, 0, 0, 1}; }
    static Value multiply(Value x, Value y) {
        return {
            (uint32_t)(((uint64_t)x.a * y.a + (uint64_t)x.b * y.c) % Mod),
            (uint32_t)(((uint64_t)x.a * y.b + (uint64_t)x.b * y.d) % Mod),
            (uint32_t)(((uint64_t)x.c * y.a + (uint64_t)x.d * y.c) % Mod),
            (uint32_t)(((uint64_t)x.c * y.b + (uint64_t)x.d * y.d) % Mod)
        };
    }
    static Value inverse(Value x) {
        return {x.d, x.b ? Mod - x.b : 0, x.c ? Mod - x.c : 0, x.a};
    }
};

template<class Group, bool Compress = true>
class PotentialUnionFind {
public:
    using Value = typename Group::Value;

private:
    std::vector<int> parent_or_size;
    std::vector<Value> potential;

    std::pair<int, Value> root_and_weight(int vertex) {
        if constexpr (Compress) {
            int parent = parent_or_size[vertex];
            if (parent < 0) return {vertex, Group::identity()};
            auto [root, parent_weight] = root_and_weight(parent);
            potential[vertex] =
                Group::multiply(parent_weight, potential[vertex]);
            parent_or_size[vertex] = root;
            return {root, potential[vertex]};
        } else {
            Value weight = Group::identity();
            while (parent_or_size[vertex] >= 0) {
                weight = Group::multiply(potential[vertex], weight);
                vertex = parent_or_size[vertex];
            }
            return {vertex, weight};
        }
    }

public:
    explicit PotentialUnionFind(int n)
        : parent_or_size(n, -1), potential(n, Group::identity()) {}

    int leader(int vertex) { return root_and_weight(vertex).first; }

    std::optional<Value> difference(int first, int second) {
        auto [first_root, first_weight] = root_and_weight(first);
        auto [second_root, second_weight] = root_and_weight(second);
        if (first_root != second_root) return std::nullopt;
        return Group::multiply(
            Group::inverse(second_weight), first_weight);
    }

    bool unite(int first, int second, Value difference) {
        auto [first_root, first_weight] = root_and_weight(first);
        auto [second_root, second_weight] = root_and_weight(second);
        if (first_root == second_root)
            return Group::multiply(
                      Group::inverse(second_weight), first_weight) ==
                   difference;
        Value edge = Group::multiply(
            Group::multiply(second_weight, difference), Group::inverse(first_weight));
        if (-parent_or_size[first_root] > -parent_or_size[second_root]) {
            std::swap(first_root, second_root);
            edge = Group::inverse(edge);
        }
        parent_or_size[second_root] += parent_or_size[first_root];
        parent_or_size[first_root] = second_root;
        potential[first_root] = edge;
        return true;
    }
};

} // namespace toy
