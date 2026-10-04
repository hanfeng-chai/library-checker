#include <toy/io.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer<1 << 20> output;
    int n = input.read_uniform<6, uint32_t>();
    std::vector<std::vector<std::pair<int, int64_t>>> graph(n);
    for (int i = 1; i < n; ++i) {
        int from = input.read_uniform<6, uint32_t>();
        int to = input.read_uniform<6, uint32_t>();
        int64_t weight = input.read_uniform<10, uint32_t>();
        graph[from].push_back({to, weight});
        graph[to].push_back({from, weight});
    }
    std::vector<int> parent(n, -1), order{0};
    std::vector<int64_t> parent_weight(n);
    for (int i = 0; i < (int)order.size(); ++i) {
        int vertex = order[i];
        for (auto [next, weight] : graph[vertex]) {
            if (next == parent[vertex]) continue;
            parent[next] = vertex;
            parent_weight[next] = weight;
            order.push_back(next);
        }
    }
    std::vector<int64_t> downward(n);
    std::vector<int> endpoint(n);
    std::iota(endpoint.begin(), endpoint.end(), 0);
    int64_t diameter = 0;
    int first = 0, second = 0;
    for (int i = n - 1; i >= 0; --i) {
        int vertex = order[i];
        int64_t best = 0, next_best = 0;
        int best_end = vertex, next_end = vertex;
        for (auto [child, weight] : graph[vertex]) {
            if (parent[child] != vertex) continue;
            int64_t candidate = downward[child] + weight;
            if (candidate > best) {
                next_best = best;
                next_end = best_end;
                best = candidate;
                best_end = endpoint[child];
            } else if (candidate > next_best) {
                next_best = candidate;
                next_end = endpoint[child];
            }
        }
        downward[vertex] = best;
        endpoint[vertex] = best_end;
        if (best + next_best > diameter) {
            diameter = best + next_best;
            first = best_end;
            second = next_end;
        }
    }
    std::vector<int> depth(n);
    for (int vertex : order)
        if (vertex) depth[vertex] = depth[parent[vertex]] + 1;
    std::vector<int> left_path, right_path;
    while (first != second) {
        if (depth[first] >= depth[second]) {
            left_path.push_back(first);
            first = parent[first];
        } else {
            right_path.push_back(second);
            second = parent[second];
        }
    }
    left_path.push_back(first);
    std::reverse(right_path.begin(), right_path.end());
    left_path.insert(left_path.end(), right_path.begin(), right_path.end());

    output.write_token((toy::u64)diameter);
    output.write_token_u32_6(left_path.size());
    for (int vertex : left_path) output.write_token_u32_6(vertex);
    output.put('\n');
}
