#include <toy/factor.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader in(toy::direct_mapping);
    toy::Writer out;
    int q = in.read_uniform<3, toy::u32>();
    while (q--) {
        auto factors = toy::factorize(in.read_uniform<19, toy::u64>());
        out.write_token_u32_6(factors.size());
        for (toy::u64 factor : factors) out.write_token(factor);
        out.put('\n');
    }
}
