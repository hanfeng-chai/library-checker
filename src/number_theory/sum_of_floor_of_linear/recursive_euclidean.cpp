#include <toy/integer.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int queries = input.read_uniform<6, toy::u32>();
    while (queries--) {
        toy::u64 n = input.read_uniform<10, toy::u32>();
        toy::u64 m = input.read_uniform<10, toy::u32>();
        toy::u64 a = input.read_uniform<10, toy::u32>();
        toy::u64 b = input.read_uniform<10, toy::u32>();
        output.writeln(toy::floor_sum_recursive(n, m, a, b));
    }
}
