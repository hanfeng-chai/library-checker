#include <toy/io.hpp>
#include <toy/range.hpp>

struct Minimum {
    toy::u32 operator()(toy::u32 a, toy::u32 b) const {
        return std::min(a, b);
    }
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::CompactWriter<> output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    for (toy::u32& value : values)
        value = input.read_uniform<10, toy::u32>();
    toy::SparseTable<toy::u32, Minimum> table(values);
    while (queries--) {
        int left = input.read_uniform<6, toy::u32>();
        int right = input.read_uniform<6, toy::u32>();
        output.writeln((toy::u64)table.fold_idempotent(left, right));
    }
}
