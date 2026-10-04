#pragma once

#include <bits/extc++.h>

namespace toy {

struct TreeDiameter {
    int64_t length;
    std::vector<int> path;
};

template <int VertexCapacity, int QueryCapacity>
class FixedOfflineLca {
    int size = 0, query_count = 0, query_edges = 0;
    std::array<int, VertexCapacity> first_child, next_sibling;
    std::array<int, VertexCapacity> first_query, disjoint_set;
    std::array<unsigned char, VertexCapacity> finished;
    std::array<int, QueryCapacity * 2> query_vertex, query_id, next_query;
    std::array<int, QueryCapacity> answer;

    int find(int vertex) {
        int root = vertex;
        while (disjoint_set[root] != root) root = disjoint_set[root];
        while (disjoint_set[vertex] != vertex) {
            int next = disjoint_set[vertex];
            disjoint_set[vertex] = root;
            vertex = next;
        }
        return root;
    }
    void visit(int vertex) {
        disjoint_set[vertex] = vertex;
        for (int child = first_child[vertex]; child != -1; child = next_sibling[child]) {
            visit(child);
            disjoint_set[child] = vertex;
        }
        finished[vertex] = true;
        for (int edge = first_query[vertex]; edge != -1; edge = next_query[edge])
            if (finished[query_vertex[edge]]) answer[query_id[edge]] = find(query_vertex[edge]);
    }

  public:
    void reset(int vertex_count, int queries) {
        size = vertex_count;
        query_count = queries;
        query_edges = 0;
        std::fill_n(first_child.data(), size, -1);
        std::fill_n(first_query.data(), size, -1);
        std::fill_n(finished.data(), size, 0);
    }
    void add_parent(int child, int parent) {
        next_sibling[child] = first_child[parent];
        first_child[parent] = child;
    }
    void add_query(int id, int first, int second) {
        query_vertex[query_edges] = second;
        query_id[query_edges] = id;
        next_query[query_edges] = first_query[first];
        first_query[first] = query_edges++;
        query_vertex[query_edges] = first;
        query_id[query_edges] = id;
        next_query[query_edges] = first_query[second];
        first_query[second] = query_edges++;
    }
    const int *solve(int root = 0) {
        visit(root);
        return answer.data();
    }
};

template <int Capacity, int BlockBits = 6>
class FixedOrderedParentLca {
    static constexpr int block_bits = BlockBits;
    static constexpr int block_size = 1 << block_bits;
    static constexpr int max_blocks = (Capacity + block_size - 1) / block_size;
    static constexpr int max_levels = std::bit_width((unsigned)max_blocks);

    int size = 0, blocks = 0;
    std::array<int, Capacity> vertex_to_order{};
    std::array<int, Capacity> order_value{};
    std::array<int, Capacity> prefix_minimum{};
    std::array<int, Capacity> suffix_minimum{};
    std::array<std::array<int, max_blocks>, max_levels> table{};

  public:
    void build(int vertex_count, const int *parents) {
        size = vertex_count;
        std::fill_n(vertex_to_order.data(), size, 0);
        for (int vertex = size - 1; vertex > 0; --vertex)
            vertex_to_order[parents[vertex - 1]] += vertex_to_order[vertex] + 1;
        for (int vertex = 1; vertex < size; ++vertex) {
            int parent = parents[vertex - 1];
            int child_subtree = vertex_to_order[vertex];
            int &parent_cursor = vertex_to_order[parent];
            int next_cursor = parent_cursor - child_subtree - 1;
            vertex_to_order[vertex] = parent_cursor;
            parent_cursor = next_cursor;
        }
        order_value[0] = 0;
        for (int vertex = 1; vertex < size; ++vertex)
            order_value[vertex_to_order[vertex]] = parents[vertex - 1];

        blocks = (size + block_size - 1) >> block_bits;
        for (int begin = 0; begin < size; begin += block_size) {
            int end = std::min(begin + block_size, size);
            int value = order_value[begin];
            for (int i = begin; i < end; ++i) {
                value = std::min(value, order_value[i]);
                prefix_minimum[i] = value;
            }
            value = order_value[end - 1];
            for (int i = end - 1; i >= begin; --i) {
                value = std::min(value, order_value[i]);
                suffix_minimum[i] = value;
            }
            table[0][begin >> block_bits] = prefix_minimum[end - 1];
        }
        for (int level = 1, half = 1; half * 2 <= blocks; ++level, half <<= 1)
            for (int i = 0; i + half * 2 <= blocks; ++i)
                table[level][i] = std::min(table[level - 1][i], table[level - 1][i + half]);
    }

    [[gnu::always_inline]] int lca(int first, int second) const {
        int left = vertex_to_order[first];
        int right = vertex_to_order[second];
        if (left > right) std::swap(left, right);
        if (left == right) return first;
        ++left;
        int left_block = left >> block_bits;
        int right_block = right >> block_bits;
        if (left_block == right_block) {
            int answer = order_value[left];
            for (int i = left + 1; i <= right; ++i) answer = std::min(answer, order_value[i]);
            return answer;
        }
        int answer = std::min(suffix_minimum[left], prefix_minimum[right]);
        int first_full = left_block + 1;
        int last_full = right_block;
        if (first_full < last_full) {
            int level = std::bit_width((unsigned)(last_full - first_full)) - 1;
            int width = 1 << level;
            answer = std::min(answer, table[level][first_full]);
            answer = std::min(answer, table[level][last_full - width]);
        }
        return answer;
    }
};

template <int Capacity>
class FixedSchieberVishkinLca {
    struct VertexInfo {
        uint32_t ascendant;
        uint32_t inlabel;
    };

    int size = 0;
    alignas(64) std::array<int, Capacity> parent{};
    alignas(64) std::array<uint32_t, Capacity> leaf_count{};
    alignas(64) std::array<VertexInfo, Capacity> info{};
    alignas(64) std::array<uint32_t, Capacity> head_parent{};

    [[gnu::always_inline]] static uint32_t lowbit(uint32_t value) { return value & (0U - value); }

  public:
    void reset(int vertex_count) {
        size = vertex_count;
        parent[0] = 0;
        leaf_count[0] = 0;
    }

    void add_parent(int vertex, int ancestor) {
        parent[vertex] = ancestor;
        leaf_count[vertex] = 0;
    }

    [[gnu::optimize("unroll-loops")]] void build() {
        for (int vertex = size - 1; vertex > 0; --vertex) {
            if (leaf_count[vertex] == 0) leaf_count[vertex] = 1;
            leaf_count[parent[vertex]] += leaf_count[vertex];
        }

        info[0].inlabel = 1;
        for (int vertex = 1; vertex < size; ++vertex) {
            int ancestor = parent[vertex];
            info[vertex].inlabel = info[ancestor].inlabel;
            info[ancestor].inlabel += leaf_count[vertex];
        }

        for (int vertex = 0; vertex < size; ++vertex) info[parent[vertex]].inlabel = 0;
        for (int vertex = size - 1; vertex >= 0; --vertex) {
            int ancestor = parent[vertex];
            if (lowbit(info[ancestor].inlabel) < lowbit(info[vertex].inlabel))
                info[ancestor].inlabel = info[vertex].inlabel;
        }
        for (int vertex = size - 1; vertex >= 0; --vertex)
            head_parent[info[vertex].inlabel] = parent[vertex];

        info[0].ascendant = 0;
        for (int vertex = 1; vertex < size; ++vertex)
            info[vertex].ascendant = info[parent[vertex]].ascendant | lowbit(info[vertex].inlabel);
    }

    void build(int vertex_count, const int *parents) {
        reset(vertex_count);
        for (int vertex = 1; vertex < size; ++vertex) add_parent(vertex, parents[vertex - 1]);
        build();
    }

    [[gnu::always_inline]] int lca(int first, int second) const {
        uint32_t difference = info[first].inlabel ^ info[second].inlabel;
        if (difference != 0) {
            uint32_t highest = std::bit_floor(difference);
            uint32_t common = info[first].ascendant & info[second].ascendant & (0U - highest);
            uint32_t branch = info[first].ascendant ^ common;
            if (branch != 0) {
                uint32_t bit = std::bit_floor(branch);
                first = head_parent[(info[first].inlabel & (0U - bit)) | bit];
            }
            branch = info[second].ascendant ^ common;
            if (branch != 0) {
                uint32_t bit = std::bit_floor(branch);
                second = head_parent[(info[second].inlabel & (0U - bit)) | bit];
            }
        }
        return std::min(first, second);
    }
};

template <class T, int Capacity>
class FixedCartesianTree {
    std::array<T, Capacity> values;
    std::array<int, Capacity> parent, stack;

  public:
    T &operator[](int index) { return values[index]; }
    const int *build(int size) {
        int stack_size = 0;
        for (int index = 0; index < size; ++index) {
            int detached = -1;
            while (stack_size && values[index] < values[stack[stack_size - 1]])
                detached = stack[--stack_size];
            parent[index] = stack_size ? stack[stack_size - 1] : -1;
            if (detached != -1) parent[detached] = index;
            stack[stack_size++] = index;
        }
        parent[stack[0]] = stack[0];
        return parent.data();
    }
};

template <int VertexCapacity, int QueryCapacity>
class FixedOfflineTreeJump {
    static constexpr int block_bits = 4;
    static constexpr int block_size = 1 << block_bits;
    static constexpr int block_capacity = (VertexCapacity + block_size - 1) / block_size + 1;
    static constexpr int level_capacity = std::bit_width((unsigned)block_capacity);

    int size = 0, query_count = 0;
    std::array<uint32_t, VertexCapacity> degree{}, parent_xor{}, order{};
    std::array<uint32_t, VertexCapacity> position{}, depth{}, prefix{}, suffix{};
    std::array<uint32_t, VertexCapacity + 1> count{};
    std::array<std::array<uint32_t, block_capacity>, level_capacity> table{};
    std::array<uint32_t, QueryCapacity> query_position{}, query_distance{};
    std::array<uint32_t, QueryCapacity> answer{}, query_order{};

    uint32_t range_minimum(uint32_t left, uint32_t right) const {
        uint32_t left_block = left >> block_bits;
        uint32_t right_block = right >> block_bits;
        if (left_block == right_block) {
            uint32_t result = depth[order[left]];
            for (uint32_t i = left + 1; i <= right; ++i) result = std::min(result, depth[order[i]]);
            return result;
        }
        uint32_t result = std::min(suffix[left], prefix[right]);
        if (left_block + 1 < right_block) {
            uint32_t level = std::bit_width(right_block - left_block - 1) - 1;
            uint32_t width = 1U << level;
            result = std::min(result, table[level][left_block + 1]);
            result = std::min(result, table[level][right_block - width]);
        }
        return result;
    }

  public:
    void reset(int vertex_count, int queries) {
        size = vertex_count;
        query_count = queries;
        std::fill_n(degree.data(), size, 0);
        std::fill_n(parent_xor.data(), size, 0);
        std::fill_n(position.data(), size, 1);
        std::fill_n(count.data(), size + 1, 0);
    }
    void add_edge(uint32_t first, uint32_t second) {
        ++degree[first];
        ++degree[second];
        parent_xor[first] ^= second;
        parent_xor[second] ^= first;
    }
    void build(uint32_t root = 0) {
        degree[root] = 0;
        uint32_t order_index = size - 1;
        for (uint32_t start = 0; start < (uint32_t)size; ++start) {
            for (uint32_t vertex = start; degree[vertex] == 1; vertex = parent_xor[vertex]) {
                uint32_t parent = parent_xor[vertex];
                order[order_index--] = vertex;
                position[parent] += position[vertex];
                --degree[vertex];
                --degree[parent];
                parent_xor[parent] ^= vertex;
            }
        }
        order[0] = root;
        depth[root] = 0;
        for (int i = 1; i < size; ++i) {
            uint32_t vertex = order[i];
            uint32_t parent = parent_xor[vertex];
            depth[vertex] = depth[parent] + 1;
            uint32_t subtree = position[vertex];
            position[vertex] = position[parent];
            position[parent] -= subtree;
        }
        for (uint32_t vertex = 0; vertex < (uint32_t)size; ++vertex) {
            uint32_t index = --position[vertex];
            order[index] = vertex;
        }
        for (int i = 0; i < size; ++i) {
            uint32_t value = depth[order[i]];
            prefix[i] = (i & (block_size - 1)) ? std::min(prefix[i - 1], value) : value;
            if ((i & (block_size - 1)) == block_size - 1) table[0][i >> block_bits] = prefix[i];
        }
        for (int i = size - 1; i >= 0; --i) {
            uint32_t value = depth[order[i]];
            suffix[i] = ((~i) & (block_size - 1)) ? std::min(suffix[i + 1], value) : value;
        }
        int blocks = (size + block_size - 1) >> block_bits;
        for (int level = 1, half = 1; half * 2 <= blocks; ++level, half <<= 1)
            for (int i = 0; i + half * 2 <= blocks; ++i)
                table[level][i] = std::min(table[level - 1][i], table[level - 1][i + half]);
    }
    void add_query(int index, uint32_t from, uint32_t to, uint32_t step) {
        uint32_t from_position = position[from];
        uint32_t to_position = position[to];
        uint32_t common_depth = depth[from];
        if (from_position < to_position)
            common_depth = range_minimum(from_position + 1, to_position) - 1;
        else if (to_position < from_position)
            common_depth = range_minimum(to_position + 1, from_position) - 1;
        uint32_t path_vertices = depth[from] + depth[to] - 2 * common_depth + 1;
        if (step <= depth[from] - common_depth) {
            query_position[index] = from_position;
            query_distance[index] = step;
        } else if (step < path_vertices) {
            query_position[index] = to_position;
            query_distance[index] = path_vertices - step - 1;
        } else {
            query_position[index] = size;
        }
        ++count[query_position[index]];
    }
    const uint32_t *solve() {
        std::partial_sum(count.begin(), count.begin() + size + 1, count.begin());
        for (int query = 0; query < query_count; ++query)
            query_order[--count[query_position[query]]] = query;

        uint32_t stack_size = 0;
        depth[0] = 0;
        for (int index = 0; index < size; ++index) {
            uint32_t vertex = order[index];
            uint32_t parent = parent_xor[vertex];
            while (depth[stack_size] != parent) --stack_size;
            depth[++stack_size] = vertex;
            for (uint32_t item = count[index]; item < count[index + 1]; ++item) {
                uint32_t query = query_order[item];
                answer[query] = depth[stack_size - query_distance[query]];
            }
        }
        for (uint32_t item = count[size]; item < (uint32_t)query_count; ++item)
            answer[query_order[item]] = std::numeric_limits<uint32_t>::max();
        return answer.data();
    }
};

template <int Capacity>
class FixedHeavyLightTree {
    struct Edge {
        int to, next;
    };
    int size = 0, edge_count = 0, timer = 0;
    std::array<Edge, Capacity * 2> edges;
    std::array<int, Capacity> first_edge, parent, depth, subtree;
    std::array<int, Capacity> heavy, head, position, vertex_at, order, pending_head;

    void decompose(int start, int chain_head) {
        int pending_size = 1;
        order[0] = start;
        pending_head[0] = chain_head;
        while (pending_size) {
            --pending_size;
            int vertex = order[pending_size];
            int current_head = pending_head[pending_size];
            for (; vertex != -1; vertex = heavy[vertex]) {
                head[vertex] = current_head;
                position[vertex] = timer;
                vertex_at[timer++] = vertex;
                for (int edge = first_edge[vertex]; edge != -1; edge = edges[edge].next) {
                    int next = edges[edge].to;
                    if (parent[next] == vertex && next != heavy[vertex]) {
                        order[pending_size] = next;
                        pending_head[pending_size++] = next;
                    }
                }
            }
        }
    }

  public:
    void reset(int vertex_count) {
        size = vertex_count;
        edge_count = timer = 0;
        std::fill_n(first_edge.data(), size, -1);
        std::fill_n(heavy.data(), size, -1);
    }
    void add_edge(int from, int to) {
        edges[edge_count] = {to, first_edge[from]};
        first_edge[from] = edge_count++;
        edges[edge_count] = {from, first_edge[to]};
        first_edge[to] = edge_count++;
    }
    void build(int root = 0) {
        int count = 1;
        order[0] = root;
        parent[root] = root;
        depth[root] = 0;
        for (int i = 0; i < count; ++i) {
            int vertex = order[i];
            for (int edge = first_edge[vertex]; edge != -1; edge = edges[edge].next) {
                int next = edges[edge].to;
                if (next == parent[vertex]) continue;
                parent[next] = vertex;
                depth[next] = depth[vertex] + 1;
                order[count++] = next;
            }
        }
        std::fill_n(subtree.data(), size, 1);
        for (int i = size - 1; i > 0; --i) {
            int vertex = order[i];
            int up = parent[vertex];
            subtree[up] += subtree[vertex];
            if (heavy[up] == -1 || subtree[heavy[up]] < subtree[vertex]) heavy[up] = vertex;
        }
        decompose(root, root);
    }
    [[gnu::always_inline]] int lca(int first, int second) const {
        if (depth[first] < 32 && depth[second] < 32) {
            while (depth[first] > depth[second]) first = parent[first];
            while (depth[second] > depth[first]) second = parent[second];
            while (first != second) first = parent[first], second = parent[second];
            return first;
        }
        while (head[first] != head[second]) {
            if (depth[head[first]] < depth[head[second]]) std::swap(first, second);
            first = parent[head[first]];
        }
        return depth[first] < depth[second] ? first : second;
    }
    [[gnu::always_inline]] int kth_ancestor(int vertex, int distance) const {
        if (distance < 32) {
            while (distance--) vertex = parent[vertex];
            return vertex;
        }
        while (distance > position[vertex] - position[head[vertex]]) {
            distance -= position[vertex] - position[head[vertex]] + 1;
            vertex = parent[head[vertex]];
        }
        return vertex_at[position[vertex] - distance];
    }
    [[gnu::always_inline]] int jump(int from, int to, int step) const {
        int common = lca(from, to);
        int upward = depth[from] - depth[common];
        int total = upward + depth[to] - depth[common];
        if (step > total) return -1;
        return step <= upward ? kth_ancestor(from, step) : kth_ancestor(to, total - step);
    }
};

template <int Capacity>
class FixedWeightedTreeDiameter {
    int size = 0;
    std::array<uint32_t, Capacity> degree{}, xor_neighbor{}, xor_weight{};
    std::array<int, Capacity> parent{}, endpoint{};
    std::array<int64_t, Capacity> downward{};

  public:
    void reset(int vertex_count) {
        size = vertex_count;
        std::fill_n(degree.data(), size, 0);
        std::fill_n(xor_neighbor.data(), size, 0);
        std::fill_n(xor_weight.data(), size, 0);
        std::fill_n(downward.data(), size, 0);
        std::iota(endpoint.begin(), endpoint.begin() + size, 0);
    }
    void add_edge(int from, int to, uint32_t weight) {
        ++degree[from];
        ++degree[to];
        xor_neighbor[from] ^= to;
        xor_neighbor[to] ^= from;
        xor_weight[from] ^= weight;
        xor_weight[to] ^= weight;
    }
    TreeDiameter solve(int root = 0) {
        ++degree[root];
        int64_t diameter = 0;
        int first = root, second = root, center = root;
        for (int start = 0; start < size; ++start) {
            int vertex = start;
            while (degree[vertex] == 1) {
                if (vertex == root) {
                    degree[vertex] = 0;
                    break;
                }
                int next = xor_neighbor[vertex];
                int64_t candidate = downward[vertex] + xor_weight[vertex];
                parent[vertex] = next;
                if (candidate + downward[next] > diameter) {
                    diameter = candidate + downward[next];
                    first = endpoint[vertex];
                    second = endpoint[next];
                    center = next;
                }
                if (candidate > downward[next]) {
                    downward[next] = candidate;
                    endpoint[next] = endpoint[vertex];
                }
                uint32_t weight = xor_weight[vertex];
                degree[vertex] = 0;
                --degree[next];
                xor_neighbor[next] ^= vertex;
                xor_weight[next] ^= weight;
                vertex = next;
            }
        }
        std::vector<int> path, suffix;
        while (first != center) path.push_back(first), first = parent[first];
        path.push_back(center);
        while (second != center) suffix.push_back(second), second = parent[second];
        path.insert(path.end(), suffix.rbegin(), suffix.rend());
        return {diameter, std::move(path)};
    }
};

class OrderedParentLca {
    static constexpr int block_bits = 6;
    static constexpr int block_size = 1 << block_bits;
    int size;
    std::vector<int> vertex_to_order;
    std::vector<int> order_value;
    std::vector<int> prefix_minimum;
    std::vector<int> suffix_minimum;
    std::vector<std::vector<int>> table;

  public:
    explicit OrderedParentLca(const std::vector<int> &parents)
        : size(parents.size() + 1), vertex_to_order(size), order_value(size), prefix_minimum(size),
          suffix_minimum(size) {
        for (int vertex = size - 1; vertex > 0; --vertex)
            vertex_to_order[parents[vertex - 1]] += vertex_to_order[vertex] + 1;
        for (int vertex = 1; vertex < size; ++vertex) {
            int parent = parents[vertex - 1];
            int child_subtree = vertex_to_order[vertex];
            int &parent_cursor = vertex_to_order[parent];
            int next_cursor = parent_cursor - child_subtree - 1;
            vertex_to_order[vertex] = parent_cursor;
            parent_cursor = next_cursor;
        }
        for (int vertex = 1; vertex < size; ++vertex)
            order_value[vertex_to_order[vertex]] = parents[vertex - 1];

        int blocks = (size + block_size - 1) >> block_bits;
        table.emplace_back(blocks, std::numeric_limits<int>::max());
        for (int begin = 0; begin < size; begin += block_size) {
            int end = std::min(begin + block_size, size);
            int value = order_value[begin];
            for (int i = begin; i < end; ++i) {
                value = std::min(value, order_value[i]);
                prefix_minimum[i] = value;
            }
            value = order_value[end - 1];
            for (int i = end - 1; i >= begin; --i) {
                value = std::min(value, order_value[i]);
                suffix_minimum[i] = value;
            }
            table[0][begin >> block_bits] = prefix_minimum[end - 1];
        }
        for (int width = 2; width <= blocks; width <<= 1) {
            int half = width >> 1;
            table.emplace_back(blocks - width + 1);
            int level = table.size() - 1;
            for (int i = 0; i + width <= blocks; ++i)
                table[level][i] = std::min(table[level - 1][i], table[level - 1][i + half]);
        }
    }

    int lca(int first, int second) const {
        int left = vertex_to_order[first];
        int right = vertex_to_order[second];
        if (left > right) std::swap(left, right);
        if (left == right) return first;
        ++left;
        int left_block = left >> block_bits;
        int right_block = right >> block_bits;
        if (left_block == right_block) {
            int answer = order_value[left];
            for (int i = left + 1; i <= right; ++i) answer = std::min(answer, order_value[i]);
            return answer;
        }
        int answer = std::min(suffix_minimum[left], prefix_minimum[right]);
        int first_full = left_block + 1;
        int last_full = right_block;
        if (first_full < last_full) {
            int level = std::bit_width((unsigned)(last_full - first_full)) - 1;
            int width = 1 << level;
            answer = std::min(answer, table[level][first_full]);
            answer = std::min(answer, table[level][last_full - width]);
        }
        return answer;
    }
};

class HeavyLightTree {
    int size;
    std::vector<int> parent, depth, subtree, heavy, head, position, vertex_at;

  public:
    HeavyLightTree(int vertex_count, const std::vector<std::pair<int, int>> &edges, int root = 0)
        : size(vertex_count), parent(size, -1), depth(size), subtree(size, 1), heavy(size, -1),
          head(size), position(size), vertex_at(size) {
        std::vector<int> offset(size + 1);
        for (auto [from, to] : edges) ++offset[from + 1], ++offset[to + 1];
        std::partial_sum(offset.begin(), offset.end(), offset.begin());
        std::vector<int> cursor = offset;
        std::vector<int> adjacent(edges.size() * 2);
        for (auto [from, to] : edges) {
            adjacent[cursor[from]++] = to;
            adjacent[cursor[to]++] = from;
        }
        std::vector<int> order(size);
        int count = 1;
        order[0] = root;
        parent[root] = root;
        for (int i = 0; i < count; ++i) {
            int vertex = order[i];
            for (int edge = offset[vertex]; edge < offset[vertex + 1]; ++edge) {
                int next = adjacent[edge];
                if (next == parent[vertex]) continue;
                parent[next] = vertex;
                depth[next] = depth[vertex] + 1;
                order[count++] = next;
            }
        }
        for (int i = size - 1; i > 0; --i) {
            int vertex = order[i];
            int up = parent[vertex];
            subtree[up] += subtree[vertex];
            if (heavy[up] == -1 || subtree[heavy[up]] < subtree[vertex]) heavy[up] = vertex;
        }
        int timer = 0;
        std::vector<std::pair<int, int>> chains{{root, root}};
        while (!chains.empty()) {
            auto [start, chain_head] = chains.back();
            chains.pop_back();
            for (int vertex = start; vertex != -1; vertex = heavy[vertex]) {
                head[vertex] = chain_head;
                position[vertex] = timer;
                vertex_at[timer++] = vertex;
                for (int edge = offset[vertex]; edge < offset[vertex + 1]; ++edge) {
                    int next = adjacent[edge];
                    if (parent[next] == vertex && next != heavy[vertex])
                        chains.push_back({next, next});
                }
            }
        }
    }

    int lca(int first, int second) const {
        if (depth[first] < 32 && depth[second] < 32) {
            while (depth[first] > depth[second]) first = parent[first];
            while (depth[second] > depth[first]) second = parent[second];
            while (first != second) {
                first = parent[first];
                second = parent[second];
            }
            return first;
        }
        while (head[first] != head[second]) {
            if (depth[head[first]] < depth[head[second]]) std::swap(first, second);
            first = parent[head[first]];
        }
        return depth[first] < depth[second] ? first : second;
    }
    int kth_ancestor(int vertex, int distance) const {
        if (distance < 32) {
            while (distance--) vertex = parent[vertex];
            return vertex;
        }
        while (distance > position[vertex] - position[head[vertex]]) {
            distance -= position[vertex] - position[head[vertex]] + 1;
            vertex = parent[head[vertex]];
        }
        return vertex_at[position[vertex] - distance];
    }
    int jump(int from, int to, int step) const {
        int common = lca(from, to);
        int upward = depth[from] - depth[common];
        int total = upward + depth[to] - depth[common];
        if (step > total) return -1;
        return step <= upward ? kth_ancestor(from, step) : kth_ancestor(to, total - step);
    }
};

template <class T>
std::vector<int> cartesian_tree_parents(const std::vector<T> &values) {
    std::vector<int> parent(values.size(), -1);
    std::vector<int> stack;
    stack.reserve(values.size());
    for (int index = 0; index < (int)values.size(); ++index) {
        int detached = -1;
        while (!stack.empty() && values[index] < values[stack.back()]) {
            detached = stack.back();
            stack.pop_back();
        }
        if (!stack.empty()) parent[index] = stack.back();
        if (detached != -1) parent[detached] = index;
        stack.push_back(index);
    }
    int root = stack.front();
    parent[root] = root;
    return parent;
}

inline TreeDiameter
weighted_tree_diameter(const std::vector<std::vector<std::pair<int, int64_t>>> &graph) {
    auto farthest = [&](int start, std::vector<int> *output_parent = nullptr) {
        std::vector<int> parent(graph.size(), -1);
        std::vector<int64_t> distance(graph.size());
        std::vector<int> stack{start};
        parent[start] = start;
        int best = start;
        while (!stack.empty()) {
            int vertex = stack.back();
            stack.pop_back();
            if (distance[best] < distance[vertex]) best = vertex;
            for (auto [next, weight] : graph[vertex]) {
                if (parent[next] != -1) continue;
                parent[next] = vertex;
                distance[next] = distance[vertex] + weight;
                stack.push_back(next);
            }
        }
        if (output_parent) *output_parent = std::move(parent);
        return std::pair{best, distance[best]};
    };
    int endpoint = farthest(0).first;
    std::vector<int> parent;
    auto [other, length] = farthest(endpoint, &parent);
    std::vector<int> path;
    for (int vertex = other;; vertex = parent[vertex]) {
        path.push_back(vertex);
        if (vertex == endpoint) break;
    }
    std::reverse(path.begin(), path.end());
    return {length, std::move(path)};
}

inline TreeDiameter
weighted_tree_diameter(int size, const std::vector<std::tuple<int, int, int64_t>> &edges) {
    std::vector<uint32_t> degree(size), xor_neighbor(size), xor_weight(size);
    for (auto [from, to, weight] : edges) {
        ++degree[from];
        ++degree[to];
        xor_neighbor[from] ^= to;
        xor_neighbor[to] ^= from;
        xor_weight[from] ^= (uint32_t)weight;
        xor_weight[to] ^= (uint32_t)weight;
    }
    std::vector<int> parent(size);
    std::vector<int64_t> downward(size);
    std::vector<int> endpoint(size);
    std::iota(endpoint.begin(), endpoint.end(), 0);
    ++degree[0];
    int64_t diameter = 0;
    int first = 0, second = 0, center = 0;
    for (int start = 0; start < size; ++start) {
        int vertex = start;
        while (degree[vertex] == 1) {
            if (vertex == 0) {
                degree[vertex] = 0;
                break;
            }
            int next = xor_neighbor[vertex];
            int64_t candidate = downward[vertex] + xor_weight[vertex];
            parent[vertex] = next;
            if (candidate + downward[next] > diameter) {
                diameter = candidate + downward[next];
                first = endpoint[vertex];
                second = endpoint[next];
                center = next;
            }
            if (candidate > downward[next]) {
                downward[next] = candidate;
                endpoint[next] = endpoint[vertex];
            }
            uint32_t weight = xor_weight[vertex];
            degree[vertex] = 0;
            --degree[next];
            xor_neighbor[next] ^= vertex;
            xor_weight[next] ^= weight;
            vertex = next;
        }
    }
    std::vector<int> path, suffix;
    while (first != center) {
        path.push_back(first);
        first = parent[first];
    }
    path.push_back(center);
    while (second != center) {
        suffix.push_back(second);
        second = parent[second];
    }
    path.insert(path.end(), suffix.rbegin(), suffix.rend());
    return {diameter, std::move(path)};
}

class BinaryLiftTree {
    std::vector<int> depth;
    std::vector<std::vector<int>> ancestor;

    void build(std::vector<int> parent) {
        int levels = std::bit_width(parent.size());
        ancestor.assign(levels, std::move(parent));
        for (int level = 1; level < levels; ++level) {
            ancestor[level].resize(depth.size());
            for (int vertex = 0; vertex < (int)depth.size(); ++vertex)
                ancestor[level][vertex] = ancestor[level - 1][ancestor[level - 1][vertex]];
        }
    }

  public:
    explicit BinaryLiftTree(const std::vector<int> &parents) : depth(parents.size() + 1) {
        std::vector<int> parent(parents.size() + 1);
        for (int vertex = 1; vertex < (int)parent.size(); ++vertex) {
            parent[vertex] = parents[vertex - 1];
            depth[vertex] = depth[parent[vertex]] + 1;
        }
        build(std::move(parent));
    }
    BinaryLiftTree(int size, const std::vector<std::pair<int, int>> &edges, int root = 0)
        : depth(size, -1) {
        std::vector<std::vector<int>> graph(size);
        for (auto [from, to] : edges) {
            graph[from].push_back(to);
            graph[to].push_back(from);
        }
        std::vector<int> parent(size);
        std::vector<int> stack{root};
        depth[root] = 0;
        while (!stack.empty()) {
            int vertex = stack.back();
            stack.pop_back();
            for (int next : graph[vertex]) {
                if (depth[next] != -1) continue;
                depth[next] = depth[vertex] + 1;
                parent[next] = vertex;
                stack.push_back(next);
            }
        }
        parent[root] = root;
        build(std::move(parent));
    }
    int kth_ancestor(int vertex, int distance) const {
        for (int level = 0; distance; ++level, distance >>= 1)
            if (distance & 1) vertex = ancestor[level][vertex];
        return vertex;
    }
    int lca(int first, int second) const {
        if (depth[first] < depth[second]) std::swap(first, second);
        first = kth_ancestor(first, depth[first] - depth[second]);
        if (first == second) return first;
        for (int level = ancestor.size() - 1; level >= 0; --level) {
            if (ancestor[level][first] != ancestor[level][second]) {
                first = ancestor[level][first];
                second = ancestor[level][second];
            }
        }
        return ancestor[0][first];
    }
    int distance(int first, int second) const {
        int common = lca(first, second);
        return depth[first] + depth[second] - 2 * depth[common];
    }
    int jump(int from, int to, int distance_from_start) const {
        int common = lca(from, to);
        int upward = depth[from] - depth[common];
        int total = upward + depth[to] - depth[common];
        if (distance_from_start > total) return -1;
        if (distance_from_start <= upward) return kth_ancestor(from, distance_from_start);
        return kth_ancestor(to, total - distance_from_start);
    }
};

} // namespace toy
