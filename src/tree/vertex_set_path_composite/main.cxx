#include <toy/io.h>
#include <toy/tree_path_affine.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    auto [n, q] = in.read_pair<6>();
    Buffer<Affine<>> values(n);
    for (auto &f : std::span(values.p, values.n)) {
        auto [a, b] = in.read_pair<9>();
        f = {a, b};
    }
    HeavyLight h(n);
    for (u32 i = 1; i < n; ++i) {
        auto [u, v] = in.read_pair<6>();
        h.add_edge(u, v);
    }
    h.build();
    u32 method = affine_path_method(h);
    auto solve = [&](auto tag) {
        using Tree = typename decltype(tag)::type;
        Tree tree(h, std::span<const Affine<>>(values));
        values = {};
        h = HeavyLight(0);
        while (q--) {
            u32 type = in.read<u32, 1>();
            if (!type) {
                u32 v = in.read<u32, 6>();
                auto [a, b] = in.read_pair<9>();
                tree.set(v, {a, b});
            } else {
                auto [u, v] = in.read_pair<6>();
                out.write(tree.apply(u, v, in.read<u32, 9>()));
            }
        }
    };
    if (!method)
        solve(std::type_identity<ParentPathAffine<>>{});
    else if (method == 1)
        solve(std::type_identity<HeavyPathAffine<>>{});
    else
        solve(std::type_identity<WeightedPathAffine<>>{});
}
