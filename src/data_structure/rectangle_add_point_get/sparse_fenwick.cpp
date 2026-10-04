#include <toy/fenwick2d.hpp>
#include <toy/io.hpp>

struct Operation {
    int type;
    int left;
    int down;
    int right;
    int up;
    toy::i64 weight;
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, int>();
    int q = input.read_uniform<6, int>();
    std::vector<Operation> initial(n);
    std::vector<Operation> operations(q);
    std::vector<std::pair<int, int>> corners;
    corners.reserve(4 * (n + q));
    auto collect = [&](const Operation &operation) {
        corners.push_back({operation.left, operation.down});
        corners.push_back({operation.right, operation.down});
        corners.push_back({operation.left, operation.up});
        corners.push_back({operation.right, operation.up});
    };
    for (Operation &operation : initial) {
        operation = {0,
                     input.read_uniform<10, int>(),
                     input.read_uniform<10, int>(),
                     input.read_uniform<10, int>(),
                     input.read_uniform<10, int>(),
                     input.read_uniform<10, toy::i64>()};
        collect(operation);
    }
    for (Operation &operation : operations) {
        operation.type = input.read_fixed<1, int>();
        operation.left = input.read_uniform<10, int>();
        operation.down = input.read_uniform<10, int>();
        if (operation.type == 0) {
            operation.right = input.read_uniform<10, int>();
            operation.up = input.read_uniform<10, int>();
            operation.weight = input.read_uniform<10, toy::i64>();
            collect(operation);
        }
    }
    toy::SparseFenwick2D<toy::i64> tree(corners);
    auto add_rectangle = [&](const Operation &operation) {
        tree.add(operation.left, operation.down, operation.weight);
        tree.add(operation.right, operation.down, -operation.weight);
        tree.add(operation.left, operation.up, -operation.weight);
        tree.add(operation.right, operation.up, operation.weight);
    };
    for (const Operation &operation : initial) add_rectangle(operation);
    for (const Operation &operation : operations) {
        if (operation.type == 0) {
            add_rectangle(operation);
        } else {
            output.writeln_i64(tree.prefix(operation.left + 1, operation.down + 1));
        }
    }
}
