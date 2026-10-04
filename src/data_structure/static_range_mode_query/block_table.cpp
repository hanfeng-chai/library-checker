#include <toy/io.hpp>
#include <toy/mode.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    for (toy::u32 &value : values) value = input.read_uniform<10, toy::u32>();
    toy::StaticRangeMode mode(values);
    while (queries--) {
        int left = input.read_uniform<6, toy::u32>();
        int right = input.read_uniform<6, toy::u32>();
        auto [value, frequency] = mode.query(left, right);
        output.write_token(value);
        output.write_token((toy::u64)frequency);
        output.put('\n');
    }
}
