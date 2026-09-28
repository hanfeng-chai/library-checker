#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping); toy::Writer<1 << 20> output;
    int n = input.read_uniform<6, uint32_t>();
    int q = input.read_uniform<6, uint32_t>();
    std::vector<std::vector<int>> children(n);
    for (int vertex = 1; vertex < n; ++vertex)
        children[input.read_uniform<6, uint32_t>()].push_back(vertex);

    std::vector<int> first(n), depth(n), euler;
    euler.reserve(2 * n - 1);
    struct Frame { int vertex, next_child; };
    std::vector<Frame> stack{{0, 0}};
    first[0] = 0;
    while (!stack.empty()) {
        Frame& frame = stack.back();
        if (frame.next_child == (int)children[frame.vertex].size()) {
            stack.pop_back();
            if (!stack.empty()) euler.push_back(stack.back().vertex);
            continue;
        }
        int child = children[frame.vertex][frame.next_child++];
        depth[child] = depth[frame.vertex] + 1;
        first[child] = euler.size() + 1;
        euler.push_back(frame.vertex);
        euler.push_back(child);
        stack.push_back({child, 0});
    }
    if (n == 1) euler.push_back(0);

    int levels = std::bit_width(euler.size());
    std::vector<std::vector<int>> table(levels);
    table[0] = euler;
    for (int level = 1; level < levels; ++level) {
        int width = 1 << level, half = width / 2;
        table[level].resize(euler.size() - width + 1);
        for (int i = 0; i + width <= (int)euler.size(); ++i) {
            int a = table[level - 1][i];
            int b = table[level - 1][i + half];
            table[level][i] = depth[a] < depth[b] ? a : b;
        }
    }
    while (q--) {
        int left = first[input.read_uniform<6, uint32_t>()];
        int right = first[input.read_uniform<6, uint32_t>()];
        if (left > right) std::swap(left, right);
        int level = std::bit_width((unsigned)(right - left + 1)) - 1;
        int a = table[level][left];
        int b = table[level][right - (1 << level) + 1];
        output.write_token_u32_6(depth[a] < depth[b] ? a : b);
    }
}
