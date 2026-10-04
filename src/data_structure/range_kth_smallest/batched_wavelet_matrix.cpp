#include <toy/io.hpp>
#include <toy/wavelet.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n);
    for (auto &value : values) value = input.read_uniform<10, toy::u32>();
    toy::CompressedWaveletMatrix matrix(values);
    std::vector<int> left(queries), right(queries), index(queries);
    for (int i = 0; i < queries; ++i) {
        left[i] = input.read_uniform<6, toy::u32>();
        right[i] = input.read_uniform<6, toy::u32>();
        index[i] = input.read_uniform<6, toy::u32>();
    }
    for (toy::u32 answer : matrix.kth_batch(std::move(left), std::move(right), std::move(index)))
        output.write_padded_u32(answer);
}
