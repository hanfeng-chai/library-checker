#include <toy/integer.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader in(toy::direct_mapping);
    toy::CompactWriter<> out;
    int q = in.read_uniform<6, toy::u32>();
    while (q--) {
        toy::u64 n = in.read_uniform<10, toy::u32>();
        toy::u64 m = in.read_uniform<10, toy::u32>();
        toy::u64 a = in.read_uniform<10, toy::u32>();
        toy::u64 b = in.read_uniform<10, toy::u32>();
        out.writeln(toy::min_mod_linear(n, m, a, b));
    }
}
