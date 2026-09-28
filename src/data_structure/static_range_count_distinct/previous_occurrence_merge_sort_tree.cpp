#include <toy/hash.hpp>
#include <toy/io.hpp>
#include <toy/wavelet.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    toy::U64HashMap last(2 * n);
    std::vector<int> previous(n);
    for (int i = 0; i < n; ++i) {
        toy::u32 value = input.read_uniform<10, toy::u32>();
        previous[i] = last.exchange(value, i + 1) - 1;
    }
    toy::MergeSortTree tree(previous);
    while (queries--) {
        int left = input.read_uniform<6, toy::u32>();
        int right = input.read_uniform<6, toy::u32>();
        output.write_padded_u32(tree.count_less(left, right, left));
    }
}
