#include <toy/integer.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::CompactWriter<> output;
    auto values = toy::enumerate_quotients(input.read_uniform<13, toy::u64>());
    output.writeln_bounded<2'000'000>(values.size());
    for (toy::u64 value : values) output.write_token_bounded<1'000'000'000'000ULL>(value);
    output.put('\n');
}
