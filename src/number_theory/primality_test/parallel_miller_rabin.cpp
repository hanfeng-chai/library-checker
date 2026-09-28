#include <toy/io.hpp>
#include <toy/prime.hpp>

int main() {
    toy::Reader in(toy::direct_mapping);
    toy::Writer out;
    int q = in.read_uniform<6, toy::u32>();
    while (q--) {
        toy::u64 n = in.read_uniform<19, toy::u64>();
        out.write(toy::is_prime_parallel(n) ? "Yes\n" : "No\n");
    }
}
