#include <toy/io.hpp>
#include <toy/ordered.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int queries = input.read_uniform<6, toy::u32>();
    toy::BinaryTrieSet<30> set(queries);
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        toy::u32 value = input.read_uniform<9, toy::u32>();
        if (type == 0) set.insert(value);
        else if (type == 1) set.erase(value);
        else output.write_padded_u32(set.min_xor(value));
    }
}
