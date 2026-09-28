#include <toy/fenwick2d.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<toy::WeightedPoint2D<toy::u32>> points(n);
    for (auto& point : points) {
        point.x = input.read_uniform<10, toy::u32>();
        point.y = input.read_uniform<10, toy::u32>();
        point.weight = input.read_uniform<10, toy::u32>();
    }
    toy::PersistentRectangleSum solver(std::move(points));
    while (query_count--) {
        toy::u32 left = input.read_uniform<10, toy::u32>();
        toy::u32 down = input.read_uniform<10, toy::u32>();
        toy::u32 right = input.read_uniform<10, toy::u32>();
        toy::u32 up = input.read_uniform<10, toy::u32>();
        output.writeln(solver.rectangle(left, down, right, up));
    }
}
