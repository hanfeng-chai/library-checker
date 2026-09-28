#include <toy/fenwick2d.hpp>
#include <toy/io.hpp>

struct Operation {
    int type;
    int x;
    int y;
    int a;
    int b;
    toy::u64 weight;
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, int>();
    int q = input.read_uniform<6, int>();
    std::vector<Operation> initial(n);
    std::vector<Operation> operations(q);
    std::vector<std::pair<int, int>> points;
    points.reserve(n + q);
    for (Operation& operation : initial) {
        operation = {
            0,
            input.read_uniform<10, int>(),
            input.read_uniform<10, int>(),
            0,
            0,
            input.read_uniform<10, toy::u64>()};
        points.push_back({operation.x, operation.y});
    }
    for (Operation& operation : operations) {
        operation.type = input.read_fixed<1, int>();
        operation.x = input.read_uniform<10, int>();
        operation.y = input.read_uniform<10, int>();
        if (operation.type == 0) {
            operation.weight = input.read_uniform<10, toy::u64>();
            points.push_back({operation.x, operation.y});
        } else {
            operation.a = input.read_uniform<10, int>();
            operation.b = input.read_uniform<10, int>();
        }
    }
    toy::SparseFenwick2D<> tree(points);
    for (const Operation& operation : initial)
        tree.add(operation.x, operation.y, operation.weight);
    for (const Operation& operation : operations) {
        if (operation.type == 0)
            tree.add(operation.x, operation.y, operation.weight);
        else
            output.writeln(
                tree.rectangle(
                    operation.x, operation.y,
                    operation.a, operation.b));
    }
}
