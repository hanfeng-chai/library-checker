#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int query_count = input.read_uniform<6, toy::u32>();
    int capacity = query_count + 1;
    std::vector<toy::u32> data(capacity);
    int front = 0;
    int size = 0;
    while (query_count--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type <= 1) {
            toy::u32 value = input.read_uniform<10, toy::u32>();
            if (type == 0) {
                front = (front + capacity - 1) % capacity;
                data[front] = value;
            } else {
                data[(front + size) % capacity] = value;
            }
            ++size;
        } else if (type == 2) {
            front = (front + 1) % capacity;
            --size;
        } else if (type == 3) {
            --size;
        } else {
            int index = input.read_uniform<6, toy::u32>();
            output.write_padded_u32(data[(front + index) % capacity]);
        }
    }
}
