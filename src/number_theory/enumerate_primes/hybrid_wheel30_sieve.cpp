#include <toy/io.hpp>
#include <toy/prime.hpp>

int main() {
    toy::Reader in(toy::direct_mapping);
    toy::CompactWriter<> out;
    toy::u32 n = in.read_uniform<9, toy::u32>();
    toy::u32 a = in.read_uniform<9, toy::u32>();
    toy::u32 b = in.read_uniform<9, toy::u32>();
    std::vector<toy::u32> selected;
    selected.reserve(1'000'000);
    toy::u64 count = toy::HybridWheel30Sieve::enumerate(
        n, [&](toy::u64, toy::u64 prime) {
            selected.push_back(prime);
        }, a, b);
    out.write_token(count);
    out.write_token(selected.size());
    out.put('\n');
    for (toy::u32 prime : selected) out.write_token(prime);
    out.put('\n');
}
