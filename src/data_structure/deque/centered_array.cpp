#include <toy/ds.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int query_count = input.read_uniform<6, toy::u32>();
    static toy::FixedCenteredDeque<toy::u32, 500'000> deque;
    while (query_count--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type <= 1) {
            toy::u32 value = input.read_uniform<10, toy::u32>();
            if (type == 0)
                deque.push_front(value);
            else
                deque.push_back(value);
        } else if (type == 2) {
            deque.pop_front();
        } else if (type == 3) {
            deque.pop_back();
        } else {
            int index = input.read_uniform<6, toy::u32>();
            output.write_padded_u32(deque[index]);
        }
    }
}
