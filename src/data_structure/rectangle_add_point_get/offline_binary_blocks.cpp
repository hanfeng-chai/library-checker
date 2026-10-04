#include <toy/fenwick2d.hpp>
#include <toy/io.hpp>

struct Operation {
    toy::u32 type;
    toy::u32 left;
    toy::u32 down;
    toy::u32 right;
    toy::u32 up;
    toy::i64 weight;
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    toy::u32 n = input.read_uniform<6, toy::u32>();
    toy::u32 q = input.read_uniform<6, toy::u32>();
    toy::OfflineRectangleAddPointGet<> solver(n + q, q);
    for (toy::u32 i = 0; i < n; ++i) {
        toy::u32 left = input.read_uniform<10, toy::u32>();
        toy::u32 down = input.read_uniform<10, toy::u32>();
        toy::u32 right = input.read_uniform<10, toy::u32>();
        toy::u32 up = input.read_uniform<10, toy::u32>();
        toy::i64 weight = input.read_uniform<10, toy::i64>();
        solver.add_rectangle(left, down, right, up, weight);
    }
    for (toy::u32 i = 0; i < q; ++i) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        toy::u32 first = input.read_uniform<10, toy::u32>();
        toy::u32 second = input.read_uniform<10, toy::u32>();
        if (type == 0) {
            toy::u32 right = input.read_uniform<10, toy::u32>();
            toy::u32 up = input.read_uniform<10, toy::u32>();
            toy::i64 weight = input.read_uniform<10, toy::i64>();
            solver.add_rectangle(first, second, right, up, weight);
        } else {
            solver.add_query(first, second);
        }
    }
    for (toy::i64 answer : solver.solve()) output.writeln_i64(answer);
}
