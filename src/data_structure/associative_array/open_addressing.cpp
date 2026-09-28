#include <toy/hash.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int queries = input.read_uniform<7, toy::u32>();
    toy::U64HashMap map(queries);
    while (queries--) {
        int type = input.read_fixed<1, toy::u32>();
        toy::u64 key = input.read_uniform<19, toy::u64>();
        if (type == 0) map.set(key, input.read_uniform<19, toy::u64>());
        else output.writeln(map.get(key));
    }
}
