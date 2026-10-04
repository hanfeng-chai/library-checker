#include <toy/affine.hpp>
#include <toy/io.hpp>

namespace {
constexpr toy::u32 mod = 998244353;
using Function = toy::Affine<mod>;
Function tree[1 << 20];
int size;

Function compose(Function first, Function second) {
    return toy::ComposeAffine<mod>{}(first, second);
}

void update(int node, int left, int right, int index, Function value) {
    if (right - left == 1) {
        tree[node] = value;
        return;
    }
    int middle = (left + right) / 2;
    if (index < middle)
        update(node * 2, left, middle, index, value);
    else
        update(node * 2 + 1, middle, right, index, value);
    tree[node] = compose(tree[node * 2], tree[node * 2 + 1]);
}

Function query(int node, int left, int right, int query_left, int query_right) {
    if (query_right <= left || right <= query_left) return {};
    if (query_left <= left && right <= query_right) return tree[node];
    int middle = (left + right) / 2;
    return compose(query(node * 2, left, middle, query_left, query_right),
                   query(node * 2 + 1, middle, right, query_left, query_right));
}
} // namespace

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    size = std::bit_ceil((unsigned)n);
    for (int i = 0; i < size; ++i) tree[size + i] = {};
    for (int i = 0; i < n; ++i)
        tree[size + i] = {input.read_uniform<9, toy::u32>(), input.read_uniform<9, toy::u32>()};
    for (int i = size - 1; i; --i) tree[i] = compose(tree[i * 2], tree[i * 2 + 1]);
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0) {
            int index = input.read_uniform<6, toy::u32>();
            Function value{input.read_uniform<9, toy::u32>(), input.read_uniform<9, toy::u32>()};
            update(1, 0, size, index, value);
        } else {
            int left = input.read_uniform<6, toy::u32>();
            int right = input.read_uniform<6, toy::u32>();
            toy::u32 x = input.read_uniform<9, toy::u32>();
            output.write_padded_u32(query(1, 0, size, left, right)(x));
        }
    }
}
