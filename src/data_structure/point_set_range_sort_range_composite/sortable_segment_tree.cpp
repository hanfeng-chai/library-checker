#include <toy/affine.hpp>
#include <toy/io.hpp>
#include <toy/sortable.hpp>

int main() {
    constexpr toy::u32 mod = 998244353;
    using Function = toy::Affine<mod>;
    struct Query {
        int type;
        int x;
        int y;
        toy::u32 a;
        toy::u32 b;
    };

    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<int> keys(n), all_keys;
    std::vector<Function> functions(n);
    all_keys.reserve(n + query_count);
    for (int i = 0; i < n; ++i) {
        keys[i] = input.read_uniform<10, toy::u32>();
        functions[i].a = input.read_uniform<9, toy::u32>();
        functions[i].b = input.read_uniform<9, toy::u32>();
        all_keys.push_back(keys[i]);
    }
    std::vector<Query> queries;
    queries.reserve(query_count);
    for (int query = 0; query < query_count; ++query) {
        int type = input.read_fixed<1, toy::u32>();
        int x = input.read_uniform<6, toy::u32>();
        int y = input.read_uniform<10, toy::u32>();
        toy::u32 a = 0;
        toy::u32 b = 0;
        if (!type) {
            a = input.read_uniform<9, toy::u32>();
            b = input.read_uniform<9, toy::u32>();
            all_keys.push_back(y);
        } else if (type == 1) {
            a = input.read_uniform<9, toy::u32>();
        }
        queries.push_back({type, x, y, a, b});
    }
    std::sort(all_keys.begin(), all_keys.end());
    all_keys.erase(std::unique(all_keys.begin(), all_keys.end()), all_keys.end());
    for (int &key : keys)
        key = std::lower_bound(all_keys.begin(), all_keys.end(), key) - all_keys.begin();
    toy::SortableSegmentTree tree(all_keys.size(), keys, functions, Function{},
                                  toy::ComposeAffine<mod>{});
    for (const Query &query : queries) {
        if (!query.type) {
            int key =
                std::lower_bound(all_keys.begin(), all_keys.end(), query.y) - all_keys.begin();
            tree.set(query.x, key, Function{query.a, query.b});
        } else if (query.type == 1) {
            output.write_padded_u32(tree.fold(query.x, query.y)(query.a));
        } else if (query.type == 2) {
            tree.sort_ascending(query.x, query.y);
        } else {
            tree.sort_descending(query.x, query.y);
        }
    }
}
