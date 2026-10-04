#include <toy/affine.hpp>
#include <toy/io.hpp>

namespace {
struct Operation {
    int type;
    int left;
    int right;
    toy::u32 b;
    toy::u32 c;
};
} // namespace

int main() {
    constexpr toy::u32 mod = 998244353;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<10, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<Operation> operations(query_count);
    std::vector<int> coordinates{0, n};
    for (Operation &operation : operations) {
        operation.type = input.read_fixed<1, toy::u32>();
        operation.left = input.read_uniform<10, toy::u32>();
        operation.right = input.read_uniform<10, toy::u32>();
        if (operation.type == 0) {
            operation.b = input.read_uniform<9, toy::u32>();
            operation.c = input.read_uniform<9, toy::u32>();
        }
        coordinates.push_back(operation.left);
        coordinates.push_back(operation.right);
    }
    std::sort(coordinates.begin(), coordinates.end());
    coordinates.erase(std::unique(coordinates.begin(), coordinates.end()), coordinates.end());
    toy::CompressedAffineSumTree<mod> tree(coordinates);
    for (const Operation &operation : operations) {
        int left = std::lower_bound(coordinates.begin(), coordinates.end(), operation.left) -
                   coordinates.begin();
        int right = std::lower_bound(coordinates.begin(), coordinates.end(), operation.right) -
                    coordinates.begin();
        if (operation.type == 0)
            tree.apply(left, right, {operation.b, operation.c});
        else
            output.write_padded_u32(tree.fold(left, right));
    }
}
