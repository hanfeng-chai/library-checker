#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer<1 << 20> output;
    int n = input.read_uniform<7, uint32_t>();
    std::vector<uint32_t> values(n);
    for (auto &value : values) value = input.read_uniform<10, uint32_t>();

    std::vector<int> parent(n, -1), increasing;
    increasing.reserve(n);
    for (int index = 0; index < n; ++index) {
        int left_root = -1;
        while (!increasing.empty() && values[index] < values[increasing.back()]) {
            left_root = increasing.back();
            increasing.pop_back();
        }
        if (!increasing.empty()) parent[index] = increasing.back();
        if (left_root != -1) parent[left_root] = index;
        increasing.push_back(index);
    }
    parent[increasing.front()] = increasing.front();
    for (int index = 0; index < n; ++index) output.write_token_u32_6(parent[index]);
    output.put('\n');
}
