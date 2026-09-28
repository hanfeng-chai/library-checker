#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    toy::u64 a = input.read_uniform<10, toy::u32>();
    toy::u64 b = input.read_uniform<10, toy::u32>();
    output.writeln(a + b);
}