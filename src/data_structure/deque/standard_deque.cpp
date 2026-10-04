#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int query_count = input.read_uniform<6, toy::u32>();
    std::deque<toy::u32> values;
    while (query_count--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0)
            values.push_front(input.read_uniform<10, toy::u32>());
        else if (type == 1)
            values.push_back(input.read_uniform<10, toy::u32>());
        else if (type == 2)
            values.pop_front();
        else if (type == 3)
            values.pop_back();
        else
            output.write_padded_u32(values[input.read_uniform<6, toy::u32>()]);
    }
}
