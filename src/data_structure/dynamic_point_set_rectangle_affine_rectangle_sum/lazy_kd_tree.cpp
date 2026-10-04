#include <bits/extc++.h>
#include <toy/io.hpp>
#include <toy/kd_tree.hpp>

struct Query {
    toy::u32 operation;
    toy::u32 first;
    toy::u32 second;
    toy::u32 third;
    toy::u32 fourth;
    toy::u32 fifth;
    toy::u32 sixth;
};

int main() {
    constexpr toy::u32 mod = 998244353;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    toy::u32 n = input.read_uniform<6, toy::u32>();
    toy::u32 q = input.read_uniform<6, toy::u32>();
    toy::LazyAffineKDTree2D<mod> tree(n + q);
    for (toy::u32 i = 0; i < n; ++i) {
        toy::u32 x = input.read_uniform<9, toy::u32>();
        toy::u32 y = input.read_uniform<9, toy::u32>();
        toy::u32 weight = input.read_uniform<9, toy::u32>();
        tree.register_point(x, y, weight, true);
    }
    std::vector<Query> queries(q);
    for (Query &query : queries) {
        query.operation = input.read_fixed<1, toy::u32>();
        if (query.operation == 0) {
            toy::u32 x = input.read_uniform<9, toy::u32>();
            toy::u32 y = input.read_uniform<9, toy::u32>();
            query.second = input.read_uniform<9, toy::u32>();
            query.first = tree.register_point(x, y);
        } else if (query.operation == 1) {
            query.first = input.read_uniform<6, toy::u32>();
            query.second = input.read_uniform<9, toy::u32>();
        } else {
            query.first = input.read_uniform<9, toy::u32>();
            query.second = input.read_uniform<9, toy::u32>();
            query.third = input.read_uniform<9, toy::u32>();
            query.fourth = input.read_uniform<9, toy::u32>();
            if (query.operation == 3) {
                query.fifth = input.read_uniform<9, toy::u32>();
                query.sixth = input.read_uniform<9, toy::u32>();
            }
        }
    }
    tree.build();
    for (const Query &query : queries) {
        if (query.operation == 0) {
            tree.activate(query.first, query.second);
        } else if (query.operation == 1) {
            tree.set(query.first, query.second);
        } else if (query.operation == 2) {
            output.write_padded_u32(
                tree.rectangle_sum(query.first, query.second, query.third, query.fourth));
        } else {
            tree.rectangle_apply(query.first, query.second, query.third, query.fourth, query.fifth,
                                 query.sixth);
        }
    }
}
