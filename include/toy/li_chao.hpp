#pragma once

#include <bits/extc++.h>

namespace toy {

struct Line {
    int64_t slope = 0;
    int64_t intercept = std::numeric_limits<int64_t>::max();
    int64_t operator()(int64_t x) const {
        if (intercept == std::numeric_limits<int64_t>::max()) return intercept;
        return (int64_t)((__int128)slope * x + intercept);
    }
};

struct CompactLine {
    static constexpr int64_t infinity = 3'000'000'000'000'000'000LL;

    int32_t slope = 0;
    int64_t intercept = infinity;

    int64_t operator()(int32_t x) const {
        return (int64_t)slope * x + intercept;
    }
};

class IndexedLiChaoTree {
    int count;
    int size;
    int height;
    std::vector<int32_t> coordinates;
    std::vector<CompactLine> lines;
    std::vector<int64_t> leaf_values;

    void add(int node, int node_height, CompactLine line) {
        int left = (node << node_height) ^ size;
        int right = left + (1 << node_height);
        int64_t line_left = line(coordinates[left]);
        int64_t line_right = line(coordinates[right]);

        while (true) {
            if (left + 1 == right) {
                leaf_values[left] =
                    std::min(leaf_values[left], line_left);
                return;
            }
            CompactLine& current = lines[node];
            int64_t current_left = current(coordinates[left]);
            int64_t current_right = current(coordinates[right]);
            if (line_left < current_left) {
                if (line_right < current_right) {
                    std::swap(line, current);
                    return;
                }
                int middle = (left + right) / 2;
                int64_t line_middle = line(coordinates[middle]);
                int64_t current_middle = current(coordinates[middle]);
                if (line_middle < current_middle) {
                    std::swap(line, current);
                    node = node * 2 + 1;
                    left = middle;
                    line_left = current_middle;
                    line_right = current_right;
                } else {
                    node *= 2;
                    right = middle;
                    line_right = line_middle;
                }
            } else {
                if (line_right < current_right) {
                    int middle = (left + right) / 2;
                    int64_t line_middle = line(coordinates[middle]);
                    int64_t current_middle = current(coordinates[middle]);
                    if (line_middle < current_middle) {
                        std::swap(line, current);
                        node *= 2;
                        right = middle;
                        line_left = current_left;
                        line_right = current_middle;
                    } else {
                        node = node * 2 + 1;
                        left = middle;
                        line_left = line_middle;
                    }
                } else {
                    return;
                }
            }
        }
    }

public:
    explicit IndexedLiChaoTree(
        std::vector<int32_t> sorted_coordinates,
        int32_t padding_coordinate = 1'000'000'005)
        : count(sorted_coordinates.size()),
          size(std::bit_ceil((unsigned)std::max(count, 1))),
          height(std::countr_zero((unsigned)size)),
          coordinates(size + 1, padding_coordinate),
          lines(2 * size),
          leaf_values(size, CompactLine::infinity) {
        std::copy(
            sorted_coordinates.begin(), sorted_coordinates.end(),
            coordinates.begin());
    }

    void add(CompactLine line) {
        if (count) add(1, height, line);
    }

    void add_segment(int left, int right, CompactLine line) {
        if (left >= right) return;
        int left_path = left + size - 1;
        int right_path = right + size;
        unsigned width =
            std::bit_width((unsigned)(left_path ^ right_path)) - 1;
        unsigned mask = (unsigned(1) << width) - 1;

        unsigned remaining = ~unsigned(left_path) & mask;
        while (remaining) {
            unsigned node_height = std::countr_zero(remaining);
            remaining &= remaining - 1;
            add((left_path >> node_height) ^ 1, node_height, line);
        }
        remaining = unsigned(right_path) & mask;
        while (remaining) {
            unsigned node_height = std::countr_zero(remaining);
            remaining &= remaining - 1;
            add((right_path >> node_height) ^ 1, node_height, line);
        }
    }

    int64_t minimum(int index) const {
        int64_t result = leaf_values[index];
        int32_t x = coordinates[index];
        for (int node = index + size; node >>= 1;)
            result = std::min(result, lines[node](x));
        return result;
    }
};

class DiscreteLiChaoTree {
    std::vector<int64_t> xs;
    std::vector<Line> tree;

    void add(int node, int left, int right, Line line) {
        int middle = (left + right) / 2;
        bool better_left = line(xs[left]) < tree[node](xs[left]);
        bool better_middle = line(xs[middle]) < tree[node](xs[middle]);
        if (better_middle) std::swap(line, tree[node]);
        if (right - left == 1) return;
        if (better_left != better_middle) add(node * 2, left, middle, line);
        else add(node * 2 + 1, middle, right, line);
    }
    void add_segment(int node, int left, int right, int query_left,
                     int query_right, Line line) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            add(node, left, right, line);
            return;
        }
        int middle = (left + right) / 2;
        add_segment(node * 2, left, middle, query_left, query_right, line);
        add_segment(node * 2 + 1, middle, right, query_left, query_right, line);
    }

public:
    explicit DiscreteLiChaoTree(std::vector<int64_t> coordinates)
        : xs(std::move(coordinates)), tree(4 * std::max<std::size_t>(xs.size(), 1)) {
        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
    }
    void add(Line line) {
        if (!xs.empty()) add(1, 0, xs.size(), line);
    }
    void add_segment(int64_t left, int64_t right, Line line) {
        int query_left = std::lower_bound(xs.begin(), xs.end(), left) - xs.begin();
        int query_right = std::lower_bound(xs.begin(), xs.end(), right) - xs.begin();
        if (query_left < query_right)
            add_segment(1, 0, xs.size(), query_left, query_right, line);
    }
    int64_t minimum(int64_t x) const {
        int index = std::lower_bound(xs.begin(), xs.end(), x) - xs.begin();
        int left = 0, right = xs.size(), node = 1;
        int64_t result = std::numeric_limits<int64_t>::max();
        while (true) {
            result = std::min(result, tree[node](x));
            if (right - left == 1) return result;
            int middle = (left + right) / 2;
            if (index < middle) node *= 2, right = middle;
            else node = node * 2 + 1, left = middle;
        }
    }
};

class DynamicLiChaoTree {
    struct Node {
        Line line;
        int left = 0;
        int right = 0;
    };
    static constexpr int64_t lower = -1'000'000'000;
    static constexpr int64_t upper = 1'000'000'001;
    std::vector<Node> nodes{{}, {}};

    void add(int node, int64_t left, int64_t right, Line line) {
        int64_t middle = std::midpoint(left, right);
        bool better_left = line(left) < nodes[node].line(left);
        bool better_middle = line(middle) < nodes[node].line(middle);
        if (better_middle) std::swap(line, nodes[node].line);
        if (right - left == 1) return;
        int child = better_left != better_middle ? nodes[node].left : nodes[node].right;
        if (!child) {
            child = nodes.size();
            if (better_left != better_middle) nodes[node].left = child;
            else nodes[node].right = child;
            nodes.push_back({});
        }
        if (better_left != better_middle) add(child, left, middle, line);
        else add(child, middle, right, line);
    }
    int ensure_child(int node, bool right_child) {
        int child = right_child ? nodes[node].right : nodes[node].left;
        if (!child) {
            child = nodes.size();
            if (right_child) nodes[node].right = child;
            else nodes[node].left = child;
            nodes.push_back({});
        }
        return child;
    }
    void add_segment(int node, int64_t left, int64_t right, int64_t query_left,
                     int64_t query_right, Line line) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            add(node, left, right, line);
            return;
        }
        int64_t middle = std::midpoint(left, right);
        if (query_left < middle)
            add_segment(ensure_child(node, false), left, middle,
                        query_left, query_right, line);
        if (middle < query_right)
            add_segment(ensure_child(node, true), middle, right,
                        query_left, query_right, line);
    }

public:
    explicit DynamicLiChaoTree(std::size_t lines = 0) { nodes.reserve(lines * 16); }
    void add(Line line) { add(1, lower, upper, line); }
    void add_segment(int64_t left, int64_t right, Line line) {
        add_segment(1, lower, upper, left, right, line);
    }
    int64_t minimum(int64_t x) const {
        int node = 1;
        int64_t left = lower, right = upper;
        int64_t result = std::numeric_limits<int64_t>::max();
        while (node) {
            result = std::min(result, nodes[node].line(x));
            int64_t middle = std::midpoint(left, right);
            if (x < middle) node = nodes[node].left, right = middle;
            else node = nodes[node].right, left = middle;
        }
        return result;
    }
};

} // namespace toy
