#include <toy/io.hpp>
#include <toy/ordered.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<int> values(n);
    for (int &value : values) value = input.read_uniform<10, int>();
    input.skip_spaces();
    toy::MinMaxHeap<int> queue(values);
    queue.reserve(n + queries);
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0)
            queue.push(input.read_uniform<10, int>());
        else {
            int value = type == 1 ? queue.pop_min() : queue.pop_max();
            if (value < 0)
                output.writeln((toy::i128)value);
            else
                output.writeln((toy::u64)value);
        }
    }
}
