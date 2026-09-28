#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::multiset<int> values;
    while (n--) values.insert(input.read_uniform<10, int>());
    input.skip_spaces();
    while (queries--) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        if (type == 0) {
            values.insert(input.read_uniform<10, int>());
        } else {
            auto iterator = type == 1 ? values.begin() : std::prev(values.end());
            int value = *iterator;
            values.erase(iterator);
            if (value < 0) output.writeln((toy::i128)value);
            else output.writeln((toy::u64)value);
        }
    }
}
