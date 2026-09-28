#include <toy/integer.hpp>
#include <toy/io.hpp>

int main() {
    toy::Reader in(toy::direct_mapping);
    toy::CompactWriter<> out;
    toy::u64 n = in.read_uniform<13, toy::u64>();
    toy::u64 root = std::sqrt((long double)n);
    while ((root + 1) <= n / (root + 1)) ++root;
    while (root > n / root) --root;
    toy::u64 small = root * root + root <= n ? root : root - 1;
    out.writeln_bounded<2'000'000>(root + small);
    for (toy::u64 value = 1; value <= small; ++value)
        out.write_token_bounded<1'000'000'000'000ULL>(value);
    for (toy::u64 divisor = root; divisor; --divisor)
        out.write_token_bounded<1'000'000'000'000ULL>(n / divisor);
    out.put('\n');
}
