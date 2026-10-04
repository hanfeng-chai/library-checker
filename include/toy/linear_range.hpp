#pragma once

#include <bits/extc++.h>

namespace toy {

class BridgeRangeLinearAddMinTree {
    using Linear = std::pair<int64_t, int64_t>;
    struct Point {
        int64_t x = 0, y = 0;
        Point operator+(const Point &other) const { return {x + other.x, y + other.y}; }
        Point operator-(const Point &other) const { return {x - other.x, y - other.y}; }
        bool operator==(const Point &) const = default;
    };
    using Bridge = std::pair<Point, Point>;

    int size, height;
    std::vector<Point> base;
    std::vector<Linear> lazy;
    std::vector<Bridge> bridge;

    static __int128 cross(Point a, Point b) { return (__int128)a.x * b.y - (__int128)a.y * b.x; }
    static Linear merge(Linear a, Linear b) { return {a.first + b.first, a.second + b.second}; }
    static Point apply(Point point, Linear line) {
        point.y += point.x * line.first + line.second;
        return point;
    }
    void rebuild(int node) {
        if (node >= size) return;
        Linear offset{};
        for (int ancestor = node; ancestor; ancestor >>= 1) offset = merge(offset, lazy[ancestor]);

        int left = node * 2, right = node * 2 + 1;
        int border = right;
        while (border < size) border <<= 1;
        border -= size;
        Linear left_add = lazy[left], right_add = lazy[right];
        while (left < size || right < size) {
            Point a = apply(apply(bridge[left].first, offset), left_add);
            Point b = apply(apply(bridge[left].second, offset), left_add);
            Point c = apply(apply(bridge[right].first, offset), right_add);
            Point d = apply(apply(bridge[right].second, offset), right_add);
            if (a != b && cross(b - a, c - a) < 0) {
                left *= 2;
                left_add = merge(left_add, lazy[left]);
            } else if (c != d && cross(c - b, d - b) < 0) {
                right = right * 2 + 1;
                right_add = merge(right_add, lazy[right]);
            } else if (a == b) {
                right *= 2;
                right_add = merge(right_add, lazy[right]);
            } else if (c == d) {
                left = left * 2 + 1;
                left_add = merge(left_add, lazy[left]);
            } else {
                __int128 c1 = cross(b - a, d - c);
                __int128 c2 = cross(b - a, b - c);
                bool take_left =
                    c1 == 0 && c2 == 0
                        ? c.x < border
                        : (__int128)c.x * c1 + (__int128)(d.x - c.x) * c2 < c1 * border;
                if (take_left) {
                    left = left * 2 + 1;
                    left_add = merge(left_add, lazy[left]);
                } else {
                    right *= 2;
                    right_add = merge(right_add, lazy[right]);
                }
            }
        }
        bridge[node] = {apply(base[left], left_add), apply(base[right], right_add)};
    }
    int64_t subtree_minimum(int node) const {
        Linear accumulated{};
        for (int ancestor = node; ancestor; ancestor >>= 1)
            accumulated = merge(accumulated, lazy[ancestor]);
        while (node < size) {
            if (apply(bridge[node].first, accumulated).y <
                apply(bridge[node].second, accumulated).y)
                node *= 2;
            else
                node = node * 2 + 1;
            accumulated = merge(accumulated, lazy[node]);
        }
        return apply(base[node], accumulated).y;
    }

  public:
    explicit BridgeRangeLinearAddMinTree(const std::vector<int64_t> &values) {
        size = 1;
        height = 0;
        while (size < (int)values.size()) size <<= 1, ++height;
        base.resize(size * 2);
        lazy.resize(size * 2);
        bridge.resize(size * 2);
        for (int i = 0; i < size; ++i)
            base[size + i] = {i, i < (int)values.size() ? values[i] : 1'000'000'000'000LL};
        for (int i = size; i < size * 2; ++i) bridge[i] = {base[i], base[i]};
        for (int i = size - 1; i; --i) rebuild(i);
    }
    void add(int left, int right, int64_t slope, int64_t intercept) {
        int lower = left + size, upper = right + size;
        while (lower < upper) {
            if (lower & 1) {
                lazy[lower] = merge(lazy[lower], {slope, intercept});
                ++lower;
            }
            lower >>= 1;
            if (upper & 1) {
                --upper;
                lazy[upper] = merge(lazy[upper], {slope, intercept});
            }
            upper >>= 1;
        }
        lower = left + size;
        upper = right + size;
        for (int level = 1; level <= height; ++level) {
            if ((lower >> level << level) != lower) rebuild(lower >> level);
            if ((upper >> level << level) != upper) rebuild((upper - 1) >> level);
        }
    }
    int64_t minimum(int left, int right) const {
        int64_t result = std::numeric_limits<int64_t>::max();
        for (left += size, right += size; left < right; left >>= 1, right >>= 1) {
            if (left & 1) result = std::min(result, subtree_minimum(left++));
            if (right & 1) result = std::min(result, subtree_minimum(--right));
        }
        return result;
    }
};

class KineticRangeLinearAddMinTree {
    static constexpr int64_t infinity = 2'000'000'000'000'000'000LL;

    struct Node {
        int64_t minimum = 0;
        int64_t slope = 0;
        int64_t forward_melt = infinity;
        int64_t backward_melt = infinity;
        int64_t lazy_slope = 0;
        int64_t lazy_constant = 0;
    };

    int length;
    std::vector<Node> data;

    void pull(int node) {
        const Node &left = data[node * 2];
        const Node &right = data[node * 2 + 1];
        Node &current = data[node];
        const Node *winner;
        const Node *other;
        if (left.minimum <= right.minimum) {
            current.minimum = left.minimum;
            current.slope = left.slope;
            winner = &left;
            other = &right;
        } else {
            current.minimum = right.minimum;
            current.slope = right.slope;
            winner = &right;
            other = &left;
        }
        current.forward_melt = std::min(left.forward_melt, right.forward_melt);
        current.backward_melt = std::min(left.backward_melt, right.backward_melt);
        if (winner->slope > other->slope)
            current.forward_melt =
                std::min(current.forward_melt,
                         (other->minimum - winner->minimum) / (winner->slope - other->slope));
        if (other->slope > winner->slope)
            current.backward_melt =
                std::min(current.backward_melt,
                         (other->minimum - winner->minimum) / (other->slope - winner->slope));
    }

    void apply_shift(int node, int64_t slope, int64_t constant) {
        Node &current = data[node];
        current.minimum += slope * current.slope + constant;
        current.forward_melt = std::min(infinity, current.forward_melt - slope);
        current.backward_melt = std::min(infinity, current.backward_melt + slope);
        current.lazy_slope += slope;
        current.lazy_constant += constant;
    }

    void add_node(int node, int left, int right, int64_t slope, int64_t constant) {
        if (right - left == 1) {
            data[node].minimum += slope * data[node].slope + constant;
            return;
        }
        bool safe =
            slope >= 0 ? slope <= data[node].forward_melt : -slope <= data[node].backward_melt;
        if (safe) {
            apply_shift(node, slope, constant);
            return;
        }
        push(node, left, right);
        int middle = (left + right) >> 1;
        add_node(node * 2, left, middle, slope, constant);
        add_node(node * 2 + 1, middle, right, slope, constant);
        pull(node);
    }

    void push(int node, int left, int right) {
        Node &current = data[node];
        if (current.lazy_slope == 0 && current.lazy_constant == 0) return;
        int middle = (left + right) >> 1;
        add_node(node * 2, left, middle, current.lazy_slope, current.lazy_constant);
        add_node(node * 2 + 1, middle, right, current.lazy_slope, current.lazy_constant);
        current.lazy_slope = 0;
        current.lazy_constant = 0;
    }

    void build(int node, int left, int right, const std::vector<int64_t> &values) {
        if (right - left == 1) {
            data[node].minimum = values[left];
            data[node].slope = left;
            return;
        }
        int middle = (left + right) >> 1;
        build(node * 2, left, middle, values);
        build(node * 2 + 1, middle, right, values);
        pull(node);
    }

    void add(int node, int left, int right, int query_left, int query_right, int64_t slope,
             int64_t constant) {
        if (query_right <= left || right <= query_left) return;
        if (query_left <= left && right <= query_right) {
            add_node(node, left, right, slope, constant);
            return;
        }
        push(node, left, right);
        int middle = (left + right) >> 1;
        add(node * 2, left, middle, query_left, query_right, slope, constant);
        add(node * 2 + 1, middle, right, query_left, query_right, slope, constant);
        pull(node);
    }

    int64_t minimum(int node, int left, int right, int query_left, int query_right) {
        if (query_right <= left || right <= query_left) return infinity;
        if (query_left <= left && right <= query_right) return data[node].minimum;
        push(node, left, right);
        int middle = (left + right) >> 1;
        return std::min(minimum(node * 2, left, middle, query_left, query_right),
                        minimum(node * 2 + 1, middle, right, query_left, query_right));
    }

  public:
    explicit KineticRangeLinearAddMinTree(const std::vector<int64_t> &values)
        : length(values.size()), data(4 * values.size()) {
        build(1, 0, length, values);
    }

    void add(int left, int right, int64_t slope, int64_t constant) {
        add(1, 0, length, left, right, slope, constant);
    }

    int64_t minimum(int left, int right) { return minimum(1, 0, length, left, right); }
};

} // namespace toy
