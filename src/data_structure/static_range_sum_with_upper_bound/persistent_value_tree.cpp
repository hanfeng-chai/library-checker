#include <toy/io.hpp>

struct Node {
    int left = 0, right = 0, count = 0;
    toy::u64 sum = 0;
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int queries = input.read_uniform<6, toy::u32>();
    std::vector<toy::u32> values(n), coordinates;
    for (auto &value : values) value = input.read_uniform<10, toy::u32>();
    coordinates = values;
    std::sort(coordinates.begin(), coordinates.end());
    coordinates.erase(std::unique(coordinates.begin(), coordinates.end()), coordinates.end());
    std::vector<Node> nodes(1);
    nodes.reserve(1 + (std::size_t)n * (std::bit_width(coordinates.size()) + 1));
    std::vector<int> roots(n + 1);
    auto update = [&](auto &&self, int old, int left, int right, int index, toy::u32 value) -> int {
        int node = nodes.size();
        nodes.push_back(nodes[old]);
        ++nodes[node].count;
        nodes[node].sum += value;
        if (right - left > 1) {
            int middle = (left + right) / 2;
            if (index < middle)
                nodes[node].left = self(self, nodes[node].left, left, middle, index, value);
            else
                nodes[node].right = self(self, nodes[node].right, middle, right, index, value);
        }
        return node;
    };
    for (int i = 0; i < n; ++i) {
        int index = std::lower_bound(coordinates.begin(), coordinates.end(), values[i]) -
                    coordinates.begin();
        roots[i + 1] = update(update, roots[i], 0, coordinates.size(), index, values[i]);
    }
    auto prefix = [&](auto &&self, int left_root, int right_root, int left, int right,
                      int end) -> std::pair<int, toy::u64> {
        if (end <= left) return {};
        if (right <= end)
            return {nodes[right_root].count - nodes[left_root].count,
                    nodes[right_root].sum - nodes[left_root].sum};
        int middle = (left + right) / 2;
        auto result = self(self, nodes[left_root].left, nodes[right_root].left, left, middle, end);
        if (middle < end) {
            auto other =
                self(self, nodes[left_root].right, nodes[right_root].right, middle, right, end);
            result.first += other.first;
            result.second += other.second;
        }
        return result;
    };
    while (queries--) {
        int left = input.read_uniform<6, toy::u32>();
        int right = input.read_uniform<6, toy::u32>();
        toy::u32 bound = input.read_uniform<10, toy::u32>();
        int end =
            std::upper_bound(coordinates.begin(), coordinates.end(), bound) - coordinates.begin();
        auto [count, sum] = prefix(prefix, roots[left], roots[right], 0, coordinates.size(), end);
        output.write_token((toy::u64)count);
        output.write_token(sum);
        output.put('\n');
    }
}
