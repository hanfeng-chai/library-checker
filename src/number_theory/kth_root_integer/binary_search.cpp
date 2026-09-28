#include <toy/integer.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::CompactWriter<> output;
    int queries = input.read_uniform<6, toy::u32>();
    while (queries--) {
        toy::u64 n = input.read_uniform<20, toy::u64>();
        unsigned k = input.read_uniform<2, toy::u32>();
        output.writeln(toy::kth_root_floor_binary(n, k));
    }
}
