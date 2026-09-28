#include <toy/io.hpp>
#include <toy/persistent.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int query_count = input.read_uniform<6, toy::u32>();
    toy::OfflinePersistentQueue<toy::u32> queue(query_count);
    for (int query = 0; query < query_count; ++query) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int base = input.read_uniform<6, int>() + 1;
        if (type == 0)
            queue.push(base, input.read_uniform<10, toy::u32>());
        else
            queue.pop(base);
    }
    for (toy::u32 value : queue.solve()) output.write_padded_u32(value);
}
