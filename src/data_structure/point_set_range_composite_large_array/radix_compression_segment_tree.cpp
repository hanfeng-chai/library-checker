#include <toy/affine.hpp>
#include <toy/io.hpp>
#include <toy/sort.hpp>

namespace {
struct Operation {
    toy::u32 type;
    toy::u32 left;
    toy::u32 right;
    toy::u32 x;
    toy::u32 y;
};
}

int main() {
    constexpr toy::u32 mod = 998244353;
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    input.read_uniform<10, toy::u32>();
    int query_count = input.read_uniform<6, int>();
    std::vector<Operation> operations(query_count);
    std::vector<toy::u64> events;
    events.reserve(2 * query_count);

    for (int index = 0; index < query_count; ++index) {
        Operation& operation = operations[index];
        operation.type = input.read_fixed<1, toy::u32>();
        operation.left = input.read_uniform<10, toy::u32>();
        if (operation.type == 0) {
            operation.x = input.read_uniform<9, toy::u32>();
            operation.y = input.read_uniform<9, toy::u32>();
            events.push_back((toy::u64)(index << 2) << 32 | operation.left);
        } else {
            operation.right = input.read_uniform<10, toy::u32>();
            operation.x = input.read_uniform<9, toy::u32>();
            events.push_back((toy::u64)(index << 2 | 1) << 32 | operation.left);
            events.push_back((toy::u64)(index << 2 | 2) << 32 | operation.right);
        }
    }

    toy::radix_sort_u32<30, 15>(
        events.begin(), events.end(),
        [](toy::u64 event) { return (toy::u32)event; });

    toy::u32 previous = std::numeric_limits<toy::u32>::max();
    int compressed_size = 0;
    for (toy::u64 event : events) {
        toy::u32 coordinate = event;
        toy::u32 tag = event >> 32;
        Operation& operation = operations[tag >> 2];
        if (tag & 3) {
            toy::u32 compressed = compressed_size - (coordinate == previous);
            (tag & 1 ? operation.left : operation.right) = compressed;
        } else {
            if (coordinate != previous) {
                ++compressed_size;
                previous = coordinate;
            }
            operation.left = compressed_size - 1;
        }
    }

    toy::AffineSegmentTree<mod> tree(compressed_size);
    for (const Operation& operation : operations) {
        if (operation.type == 0) {
            tree.set(operation.left, {operation.x, operation.y});
        } else {
            toy::u32 answer = operation.left == operation.right
                ? operation.x
                : tree.apply(operation.left, operation.right, operation.x);
            output.write_padded_u32(answer);
        }
    }
}
