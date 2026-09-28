#include <toy/integer.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader in(toy::direct_mapping);
    toy::CompactWriter<> out;
    int q = in.read_uniform<6, toy::u32>();
    while (q--) {
        toy::u64 n = in.read_uniform<20, toy::u64>();
        unsigned k = in.read_uniform<2, toy::u32>();
        out.writeln(toy::kth_root_floor(n, k));
    }
}
