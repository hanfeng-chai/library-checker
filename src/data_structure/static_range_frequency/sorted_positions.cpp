#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<std::pair<int, int>> positions;
    positions.reserve(n);
    for (int index = 0; index < n; ++index)
        positions.push_back({input.read_uniform<10, int>(), index});
    std::sort(positions.begin(), positions.end());
    if (queries && n == 0) input.skip_spaces();
    while (queries--) {
        int left = input.read_uniform<6, int>();
        int right = input.read_uniform<6, int>();
        int value = input.read_uniform<10, int>();
        auto first = std::lower_bound(positions.begin(), positions.end(), std::pair{value, left});
        auto last = std::lower_bound(positions.begin(), positions.end(), std::pair{value, right});
        output.writeln((toy::u64)(last - first));
    }
}
