#include <toy/fenwick2d.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    toy::OfflineRectangleSum solver(n, query_count);
    for (int i = 0; i < n; ++i) {
        toy::u32 x = input.read_uniform<10, toy::u32>();
        toy::u32 y = input.read_uniform<10, toy::u32>();
        toy::u32 weight = input.read_uniform<10, toy::u32>();
        solver.add_point(x, y, weight);
    }
    for (int i = 0; i < query_count; ++i) {
        toy::u32 left = input.read_uniform<10, toy::u32>();
        toy::u32 down = input.read_uniform<10, toy::u32>();
        toy::u32 right = input.read_uniform<10, toy::u32>();
        toy::u32 up = input.read_uniform<10, toy::u32>();
        solver.add_query(left, down, right, up);
    }
    for (toy::u64 answer : solver.solve())
        output.writeln(answer);
}
