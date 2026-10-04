#include <toy/io.hpp>
#include <toy/prime.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::CompactWriter<> output;
    toy::u32 n = input.read_uniform<9, toy::u32>();
    toy::u32 a = input.read_uniform<9, toy::u32>();
    toy::u32 b = input.read_uniform<9, toy::u32>();
    std::vector<toy::u32> selected;
    toy::u64 count = toy::OddSegmentedSieve::enumerate(n, [&](toy::u64 index, toy::u64 prime) {
        if (index % a == b) selected.push_back(prime);
    });
    output.write_token(count);
    output.write_token(selected.size());
    output.put('\n');
    for (toy::u32 prime : selected) output.write_token(prime);
    output.put('\n');
}
