#include <toy/fenwick2d.hpp>
#include <toy/io.hpp>

struct Operation {
    toy::u32 type;
    toy::u32 first;
    toy::u32 second;
    toy::u32 third;
    toy::u32 fourth;
    toy::u32 point_id;
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    toy::u32 n = input.read_uniform<6, toy::u32>();
    toy::u32 q = input.read_uniform<6, toy::u32>();
    toy::WaveletFenwickRectangleSum<> tree(n + q);
    for (toy::u32 i = 0; i < n; ++i) {
        toy::u32 x = input.read_uniform<10, toy::u32>();
        toy::u32 y = input.read_uniform<10, toy::u32>();
        toy::u64 weight = input.read_uniform<10, toy::u64>();
        tree.register_point(x, y, weight);
    }

    std::vector<Operation> operations(q);
    for (Operation& operation : operations) {
        operation.type = input.read_fixed<1, toy::u32>();
        operation.first = input.read_uniform<10, toy::u32>();
        operation.second = input.read_uniform<10, toy::u32>();
        if (operation.type == 0) {
            operation.third = input.read_uniform<10, toy::u32>();
            operation.point_id = tree.register_point(
                operation.first, operation.second);
        } else {
            operation.third = input.read_uniform<10, toy::u32>();
            operation.fourth = input.read_uniform<10, toy::u32>();
        }
    }

    tree.build();
    for (const Operation& operation : operations) {
        if (operation.type == 0) {
            tree.add(operation.point_id, operation.third);
        } else {
            output.writeln(
                tree.rectangle(
                    operation.first, operation.second,
                    operation.third, operation.fourth));
        }
    }
}
