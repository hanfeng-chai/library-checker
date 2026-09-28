#include <toy/io.hpp>
#include <toy/prime.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int queries = input.read_uniform<6, toy::u32>();
    while (queries--)
        output.write(toy::is_prime(input.read_uniform<19, toy::u64>())
            ? "Yes\n" : "No\n");
}
