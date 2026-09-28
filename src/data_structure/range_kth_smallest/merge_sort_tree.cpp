#include <toy/io.hpp>
#include <toy/wavelet.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    for (auto& value : values) value = input.read_uniform<10, toy::u32>();
    toy::MergeSortTree tree(values);
    while (queries--) {
        int left = input.read_uniform<6, toy::u32>();
        int right = input.read_uniform<6, toy::u32>();
        int index = input.read_uniform<6, toy::u32>();
        output.write_padded_u32(tree.kth(left, right, index));
    }
}
