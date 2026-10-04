#include <toy/factor.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader in(toy::direct_mapping);
    toy::Writer out;
    int q = in.read_uniform<3, toy::u32>();
    while (q--) out.writeln(toy::primitive_root_prime(in.read_uniform<19, toy::u64>()));
}
