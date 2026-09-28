#pragma once

#include <bits/extc++.h>

namespace toy {

template<uint32_t Mod>
class LazyAffineKDTree2D {
    struct BuildPoint {
        int32_t x;
        int32_t y;
        uint32_t id;
    };

    struct Node {
        int32_t x = 0;
        int32_t y = 0;
        uint32_t id = 0;
        uint32_t value = 0;
        uint32_t sum = 0;
        uint32_t active_size = 0;
        int32_t min_x = 0;
        int32_t max_x = 0;
        int32_t min_y = 0;
        int32_t max_y = 0;
        uint32_t left = 0;
        uint32_t right = 0;
        uint32_t parent = 0;
        uint32_t lazy_multiplier = 1;
        uint32_t lazy_constant = 0;
        uint32_t active = 0;
    };

    std::vector<BuildPoint> build_points;
    std::vector<uint32_t> initial_values;
    std::vector<uint8_t> initial_active;
    std::vector<Node> nodes;
    std::vector<uint32_t> positions;
    uint32_t root = 0;

    static uint32_t add_mod(uint32_t left, uint32_t right) {
        uint32_t sum = left + right;
        return sum >= Mod ? sum - Mod : sum;
    }

    static uint32_t multiply_mod(uint32_t left, uint32_t right) {
        return uint64_t(left) * right % Mod;
    }

    static void pull(Node* data, uint32_t index) {
        Node& node = data[index];
        node.sum = node.value;
        node.active_size = node.active;
        if (node.left != 0) {
            node.sum = add_mod(node.sum, data[node.left].sum);
            node.active_size += data[node.left].active_size;
        }
        if (node.right != 0) {
            node.sum = add_mod(node.sum, data[node.right].sum);
            node.active_size += data[node.right].active_size;
        }
    }

    static void apply_node(
        Node* data, uint32_t index,
        uint32_t multiplier, uint32_t constant) {
        Node& node = data[index];
        node.lazy_multiplier =
            multiply_mod(node.lazy_multiplier, multiplier);
        node.lazy_constant = add_mod(
            multiply_mod(node.lazy_constant, multiplier), constant);
        node.sum = add_mod(
            multiply_mod(node.sum, multiplier),
            uint64_t(node.active_size) * constant % Mod);
        if (node.active)
            node.value = add_mod(
                multiply_mod(node.value, multiplier), constant);
    }

    static void push(Node* data, uint32_t index) {
        Node& node = data[index];
        if (node.lazy_multiplier == 1 && node.lazy_constant == 0)
            return;
        if (node.left != 0)
            apply_node(
                data, node.left,
                node.lazy_multiplier, node.lazy_constant);
        if (node.right != 0)
            apply_node(
                data, node.right,
                node.lazy_multiplier, node.lazy_constant);
        node.lazy_multiplier = 1;
        node.lazy_constant = 0;
    }

    uint32_t build_recursive(
        uint32_t left, uint32_t right,
        bool split_x, uint32_t parent) {
        if (left == right) return 0;
        uint32_t middle = (left + right - 1) >> 1;
        if (split_x) {
            std::nth_element(
                build_points.begin() + left,
                build_points.begin() + middle,
                build_points.begin() + right,
                [](const BuildPoint& first, const BuildPoint& second) {
                    return first.x < second.x;
                });
        } else {
            std::nth_element(
                build_points.begin() + left,
                build_points.begin() + middle,
                build_points.begin() + right,
                [](const BuildPoint& first, const BuildPoint& second) {
                    return first.y < second.y;
                });
        }

        uint32_t index = middle + 1;
        const BuildPoint& point = build_points[middle];
        Node& node = nodes[index];
        node.x = node.min_x = node.max_x = point.x;
        node.y = node.min_y = node.max_y = point.y;
        node.id = point.id;
        node.value = initial_values[point.id];
        node.active = initial_active[point.id];
        node.parent = parent;
        positions[point.id] = index;
        node.left = build_recursive(
            left, middle, !split_x, index);
        node.right = build_recursive(
            middle + 1, right, !split_x, index);
        if (node.left != 0) {
            const Node& child = nodes[node.left];
            node.min_x = std::min(node.min_x, child.min_x);
            node.max_x = std::max(node.max_x, child.max_x);
            node.min_y = std::min(node.min_y, child.min_y);
            node.max_y = std::max(node.max_y, child.max_y);
        }
        if (node.right != 0) {
            const Node& child = nodes[node.right];
            node.min_x = std::min(node.min_x, child.min_x);
            node.max_x = std::max(node.max_x, child.max_x);
            node.min_y = std::min(node.min_y, child.min_y);
            node.max_y = std::max(node.max_y, child.max_y);
        }
        pull(nodes.data(), index);
        return index;
    }

    static bool disjoint(
        const Node& node,
        int32_t left, int32_t down,
        int32_t right, int32_t up) {
        return node.max_x < left || right < node.min_x ||
               node.max_y < down || up < node.min_y;
    }

    static bool covered(
        const Node& node,
        int32_t left, int32_t down,
        int32_t right, int32_t up) {
        return left <= node.min_x && node.max_x <= right &&
               down <= node.min_y && node.max_y <= up;
    }

    static uint32_t rectangle_sum_recursive(
        Node* data, uint32_t index,
        int32_t left, int32_t down,
        int32_t right, int32_t up) {
        if (disjoint(data[index], left, down, right, up))
            return 0;
        if (covered(data[index], left, down, right, up))
            return data[index].sum;
        push(data, index);
        const Node& node = data[index];
        uint32_t result =
            node.active && left <= node.x && node.x <= right &&
                    down <= node.y && node.y <= up
                ? node.value
                : 0;
        if (node.left != 0)
            result = add_mod(
                result,
                rectangle_sum_recursive(
                    data, node.left, left, down, right, up));
        if (node.right != 0)
            result = add_mod(
                result,
                rectangle_sum_recursive(
                    data, node.right, left, down, right, up));
        return result;
    }

    static void rectangle_apply_recursive(
        Node* data, uint32_t index,
        int32_t left, int32_t down,
        int32_t right, int32_t up,
        uint32_t multiplier, uint32_t constant) {
        if (disjoint(data[index], left, down, right, up))
            return;
        if (covered(data[index], left, down, right, up)) {
            apply_node(data, index, multiplier, constant);
            return;
        }
        push(data, index);
        Node& node = data[index];
        if (node.active && left <= node.x && node.x <= right &&
            down <= node.y && node.y <= up)
            node.value = add_mod(
                multiply_mod(node.value, multiplier), constant);
        if (node.left != 0)
            rectangle_apply_recursive(
                data, node.left, left, down, right, up,
                multiplier, constant);
        if (node.right != 0)
            rectangle_apply_recursive(
                data, node.right, left, down, right, up,
                multiplier, constant);
        pull(data, index);
    }

    void set_value(uint32_t point_id, uint32_t value, bool activate) {
        Node* data = nodes.data();
        uint32_t index = positions[point_id];
        std::array<uint32_t, 64> path;
        uint32_t length = 0;
        for (uint32_t current = index; current != 0;
             current = data[current].parent)
            path[length++] = current;
        for (uint32_t i = length; i-- > 0;) push(data, path[i]);
        data[index].value = value;
        if (activate) data[index].active = true;
        for (uint32_t i = 0; i < length; ++i)
            pull(data, path[i]);
    }

public:
    explicit LazyAffineKDTree2D(std::size_t point_capacity = 0) {
        build_points.reserve(point_capacity);
        initial_values.reserve(point_capacity);
        initial_active.reserve(point_capacity);
    }

    uint32_t register_point(
        int32_t x, int32_t y,
        uint32_t initial_value = 0, bool active = false) {
        uint32_t id = build_points.size();
        build_points.push_back({x, y, id});
        initial_values.push_back(initial_value);
        initial_active.push_back(active);
        return id;
    }

    void build() {
        uint32_t size = build_points.size();
        nodes.resize(size + 1);
        positions.resize(size);
        root = build_recursive(0, size, false, 0);
        build_points.clear();
        build_points.shrink_to_fit();
        initial_values.clear();
        initial_values.shrink_to_fit();
        initial_active.clear();
        initial_active.shrink_to_fit();
    }

    void activate(uint32_t point_id, uint32_t value) {
        set_value(point_id, value, true);
    }

    void set(uint32_t point_id, uint32_t value) {
        set_value(point_id, value, false);
    }

    uint32_t rectangle_sum(
        int32_t left, int32_t down,
        int32_t right, int32_t up) {
        return root == 0 || left >= right || down >= up
                   ? 0
                   : rectangle_sum_recursive(
                         nodes.data(), root,
                         left, down, right - 1, up - 1);
    }

    void rectangle_apply(
        int32_t left, int32_t down,
        int32_t right, int32_t up,
        uint32_t multiplier, uint32_t constant) {
        if (root != 0 && left < right && down < up)
            rectangle_apply_recursive(
                nodes.data(), root,
                left, down, right - 1, up - 1,
                multiplier, constant);
    }
};

} // namespace toy
