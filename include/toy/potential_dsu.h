#pragma once
#include <toy/buffer.h>

namespace toy {
// weight[x] satisfies value[x] = value[parent[x]] * weight[x].
// Operation order is preserved for noncommutative groups.
template<class Group> struct PotentialDSU {
    using Value = typename Group::Value;
    Buffer<i32> parent;
    Buffer<Value> weight;
    explicit PotentialDSU(usize n) : parent(n), weight(n) { std::fill(parent.p, parent.p + n, -1); }
    std::pair<int, Value> root(int x) {
        int p = parent[x];
        if (p < 0) return {x, Group::identity()};
        if (parent[p] < 0) return {p, weight[x]};
        auto [r, above] = root(p);
        weight[x] = Group::multiply(above, weight[x]); parent[x] = r;
        return {r, weight[x]};
    }
    std::optional<Value> difference(int a, int b) {
        auto [ra, wa] = root(a); auto [rb, wb] = root(b);
        if (ra != rb) return std::nullopt;
        return Group::multiply(Group::inverse(wb), wa);
    }
    bool merge(int a, int b, Value delta) {
        auto [ra, wa] = root(a); auto [rb, wb] = root(b);
        if (ra == rb) return Group::multiply(Group::inverse(wb), wa) == delta;
        Value edge = Group::multiply(Group::multiply(wb, delta), Group::inverse(wa));
        if (parent[ra] < parent[rb]) { std::swap(ra, rb); edge = Group::inverse(edge); }
        parent[rb] += parent[ra]; parent[ra] = rb; weight[ra] = edge;
        return true;
    }
};
}
