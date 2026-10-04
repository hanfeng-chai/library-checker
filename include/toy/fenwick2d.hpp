#pragma once

#include <bits/extc++.h>
#include <toy/ds.hpp>
#include <toy/sort.hpp>

namespace toy {

template <class T = uint64_t>
class SparseFenwick2D {
    std::vector<int> xs;
    std::vector<std::vector<int>> ys;
    std::vector<std::vector<T>> data;

  public:
    explicit SparseFenwick2D(const std::vector<std::pair<int, int>> &points) {
        xs.reserve(points.size());
        for (auto [x, y] : points) xs.push_back(x);
        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
        ys.resize(xs.size() + 1);
        for (auto [x, y] : points) {
            int index = std::lower_bound(xs.begin(), xs.end(), x) - xs.begin() + 1;
            for (; index < (int)ys.size(); index += index & -index) ys[index].push_back(y);
        }
        data.resize(ys.size());
        for (int index = 1; index < (int)ys.size(); ++index) {
            auto &coordinates = ys[index];
            std::sort(coordinates.begin(), coordinates.end());
            coordinates.erase(std::unique(coordinates.begin(), coordinates.end()),
                              coordinates.end());
            data[index].resize(coordinates.size() + 1);
        }
    }

    void add(int x, int y, T value) {
        int index = std::lower_bound(xs.begin(), xs.end(), x) - xs.begin() + 1;
        for (; index < (int)ys.size(); index += index & -index) {
            int position =
                std::lower_bound(ys[index].begin(), ys[index].end(), y) - ys[index].begin() + 1;
            for (; position < (int)data[index].size(); position += position & -position)
                data[index][position] += value;
        }
    }

    T prefix(int x, int y) const {
        int index = std::lower_bound(xs.begin(), xs.end(), x) - xs.begin();
        T result = 0;
        for (; index; index -= index & -index) {
            int position =
                std::lower_bound(ys[index].begin(), ys[index].end(), y) - ys[index].begin();
            for (; position; position -= position & -position) result += data[index][position];
        }
        return result;
    }

    T rectangle(int left, int down, int right, int up) const {
        return prefix(right, up) - prefix(left, up) - prefix(right, down) + prefix(left, down);
    }
};

template <class Weight>
struct WeightedPoint2D {
    uint32_t x;
    uint32_t y;
    Weight weight;
};

template <class Weight = uint64_t, class Sum = uint64_t>
class OfflineRectangleSum {
    struct Query {
        uint32_t left;
        uint32_t down;
        uint32_t right;
        uint32_t up;
    };

    struct Event {
        uint32_t x;
        uint32_t down;
        uint32_t up;
        uint32_t query;
        bool subtract;
    };

    std::vector<WeightedPoint2D<Weight>> points;
    std::vector<Query> queries;

  public:
    explicit OfflineRectangleSum(std::size_t point_capacity = 0, std::size_t query_capacity = 0) {
        points.reserve(point_capacity);
        queries.reserve(query_capacity);
    }

    void add_point(uint32_t x, uint32_t y, Weight weight) { points.push_back({x, y, weight}); }

    void add_query(uint32_t left, uint32_t down, uint32_t right, uint32_t up) {
        queries.push_back({left, down, right, up});
    }

    std::vector<Sum> solve() {
        radix_sort_u32(points.begin(), points.end(), [](const auto &point) { return point.y; });
        std::vector<uint32_t> ys;
        ys.reserve(points.size());
        for (auto &point : points) {
            if (ys.empty() || ys.back() != point.y) ys.push_back(point.y);
            point.y = ys.size() - 1;
        }
        radix_sort_u32(points.begin(), points.end(), [](const auto &point) { return point.x; });

        std::vector<Event> events;
        events.reserve(2 * queries.size());
        for (uint32_t i = 0; i < queries.size(); ++i) {
            const Query &query = queries[i];
            uint32_t down = std::lower_bound(ys.begin(), ys.end(), query.down) - ys.begin();
            uint32_t up = std::lower_bound(ys.begin(), ys.end(), query.up) - ys.begin();
            if (down == up) continue;
            events.push_back({query.left, down, up, i, true});
            events.push_back({query.right, down, up, i, false});
        }
        radix_sort_u32(events.begin(), events.end(), [](const Event &event) { return event.x; });

        FenwickTree<Sum> tree((int)ys.size());
        std::vector<Sum> answers(queries.size());
        std::size_t point = 0;
        for (const Event &event : events) {
            while (point < points.size() && points[point].x < event.x) {
                tree.add(points[point].y, (Sum)points[point].weight);
                ++point;
            }
            Sum value = tree.prefix_sum(event.up) - tree.prefix_sum(event.down);
            if (event.subtract)
                answers[event.query] -= value;
            else
                answers[event.query] += value;
        }
        return answers;
    }
};

template <class Weight = uint64_t, class Sum = uint64_t>
class PersistentRectangleSum {
    struct Node {
        uint32_t left = 0;
        uint32_t right = 0;
        Sum sum = 0;
    };

    int y_size;
    std::vector<uint32_t> xs;
    std::vector<uint32_t> ys;
    std::vector<Node> nodes{{}};
    std::vector<uint32_t> roots;

    uint32_t add(uint32_t old, int left, int right, int index, Weight weight) {
        uint32_t node = nodes.size();
        nodes.push_back(nodes[old]);
        nodes[node].sum += (Sum)weight;
        if (right - left == 1) return node;
        int middle = (left + right) / 2;
        if (index < middle)
            nodes[node].left = add(nodes[node].left, left, middle, index, weight);
        else
            nodes[node].right = add(nodes[node].right, middle, right, index, weight);
        return node;
    }

    Sum prefix(uint32_t before, uint32_t after, int left, int right, int end) const {
        if (end <= left) return 0;
        if (right <= end) return nodes[after].sum - nodes[before].sum;
        int middle = (left + right) / 2;
        Sum result = prefix(nodes[before].left, nodes[after].left, left, middle, end);
        if (middle < end)
            result += prefix(nodes[before].right, nodes[after].right, middle, right, end);
        return result;
    }

  public:
    explicit PersistentRectangleSum(std::vector<WeightedPoint2D<Weight>> points) {
        radix_sort_u32(points.begin(), points.end(), [](const auto &point) { return point.y; });
        ys.reserve(points.size());
        for (auto &point : points) {
            if (ys.empty() || ys.back() != point.y) ys.push_back(point.y);
            point.y = ys.size() - 1;
        }
        y_size = std::bit_ceil((unsigned)std::max<std::size_t>(ys.size(), 1));
        radix_sort_u32(points.begin(), points.end(), [](const auto &point) { return point.x; });
        xs.reserve(points.size());
        roots.resize(points.size() + 1);
        nodes.reserve(1 + points.size() * (std::bit_width((unsigned)y_size) + 1));
        for (int i = 0; i < (int)points.size(); ++i) {
            xs.push_back(points[i].x);
            roots[i + 1] = add(roots[i], 0, y_size, points[i].y, points[i].weight);
        }
    }

    Sum rectangle(uint32_t left, uint32_t down, uint32_t right, uint32_t up) const {
        int first = std::lower_bound(xs.begin(), xs.end(), left) - xs.begin();
        int last = std::lower_bound(xs.begin(), xs.end(), right) - xs.begin();
        int low = std::lower_bound(ys.begin(), ys.end(), down) - ys.begin();
        int high = std::lower_bound(ys.begin(), ys.end(), up) - ys.begin();
        return prefix(roots[first], roots[last], 0, y_size, high) -
               prefix(roots[first], roots[last], 0, y_size, low);
    }
};

template <uint32_t Mod>
class OfflineRectangleAddRectangleSum {
    struct Rectangle {
        uint32_t left;
        uint32_t down;
        uint32_t right;
        uint32_t up;
        uint32_t weight;
    };

    struct Query {
        uint32_t left;
        uint32_t down;
        uint32_t right;
        uint32_t up;
    };

    struct UpdateEvent {
        uint32_t x;
        uint32_t down;
        uint32_t up;
        uint32_t weight;
        bool remove;
    };

    struct QueryEvent {
        uint32_t x;
        uint32_t down;
        uint32_t up;
        uint32_t query;
        bool subtract;
    };

    struct Coefficient {
        uint64_t xy = 0;
        uint64_t x = 0;
        uint64_t y = 0;
        uint64_t constant = 0;

        Coefficient &operator+=(const Coefficient &other) {
            xy += other.xy;
            x += other.x;
            y += other.y;
            constant += other.constant;
            return *this;
        }
    };

    std::vector<Rectangle> rectangles;
    std::vector<Query> queries;

    static uint32_t negate(uint32_t value) { return value ? Mod - value : 0; }

    static Coefficient corner(uint32_t coefficient, uint32_t x, uint32_t y) {
        uint64_t x_mod = x % Mod;
        uint64_t y_mod = y % Mod;
        uint64_t negative_x = (uint64_t)negate(coefficient) * y_mod % Mod;
        uint64_t negative_y = (uint64_t)negate(coefficient) * x_mod % Mod;
        return {coefficient, negative_x, negative_y,
                (uint64_t)coefficient * x_mod % Mod * y_mod % Mod};
    }

    static uint32_t evaluate(const Coefficient &coefficient, uint32_t x, uint32_t y) {
        uint64_t x_mod = x % Mod;
        uint64_t y_mod = y % Mod;
        uint64_t result = coefficient.xy % Mod * x_mod % Mod * y_mod % Mod;
        result += coefficient.x % Mod * x_mod % Mod;
        result += coefficient.y % Mod * y_mod % Mod;
        result += coefficient.constant % Mod;
        return result % Mod;
    }

  public:
    explicit OfflineRectangleAddRectangleSum(std::size_t rectangle_capacity = 0,
                                             std::size_t query_capacity = 0) {
        rectangles.reserve(rectangle_capacity);
        queries.reserve(query_capacity);
    }

    void add_rectangle(uint32_t left, uint32_t down, uint32_t right, uint32_t up, uint32_t weight) {
        rectangles.push_back({left, down, right, up, weight});
    }

    void add_query(uint32_t left, uint32_t down, uint32_t right, uint32_t up) {
        queries.push_back({left, down, right, up});
    }

    std::vector<uint32_t> solve() const {
        std::vector<uint32_t> ys;
        ys.reserve(2 * rectangles.size());
        for (const Rectangle &rectangle : rectangles) {
            ys.push_back(rectangle.down);
            ys.push_back(rectangle.up);
        }
        radix_sort_u32(ys.begin(), ys.end(), [](uint32_t value) { return value; });
        ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

        std::vector<UpdateEvent> updates;
        updates.reserve(2 * rectangles.size());
        for (const Rectangle &rectangle : rectangles) {
            uint32_t down = std::lower_bound(ys.begin(), ys.end(), rectangle.down) - ys.begin();
            uint32_t up = std::lower_bound(ys.begin(), ys.end(), rectangle.up) - ys.begin();
            updates.push_back({rectangle.left, down, up, rectangle.weight, false});
            updates.push_back({rectangle.right, down, up, rectangle.weight, true});
        }
        radix_sort_u32(updates.begin(), updates.end(),
                       [](const UpdateEvent &event) { return event.x; });

        std::vector<QueryEvent> events;
        events.reserve(2 * queries.size());
        for (uint32_t i = 0; i < queries.size(); ++i) {
            uint32_t down = std::lower_bound(ys.begin(), ys.end(), queries[i].down) - ys.begin();
            uint32_t up = std::lower_bound(ys.begin(), ys.end(), queries[i].up) - ys.begin();
            events.push_back({queries[i].left, down, up, i, true});
            events.push_back({queries[i].right, down, up, i, false});
        }
        radix_sort_u32(events.begin(), events.end(),
                       [](const QueryEvent &event) { return event.x; });

        FenwickTree<Coefficient> tree((int)ys.size());
        std::vector<uint32_t> answers(queries.size());
        std::size_t update = 0;
        for (const QueryEvent &event : events) {
            while (update < updates.size() && updates[update].x < event.x) {
                const UpdateEvent &current = updates[update++];
                uint32_t coefficient = current.remove ? negate(current.weight) : current.weight;
                tree.add(current.down, corner(coefficient, current.x, ys[current.down]));
                tree.add(current.up, corner(negate(coefficient), current.x, ys[current.up]));
            }
            uint32_t upper = evaluate(tree.prefix_sum(event.up), event.x, queries[event.query].up);
            uint32_t lower =
                evaluate(tree.prefix_sum(event.down), event.x, queries[event.query].down);
            uint32_t value = upper >= lower ? upper - lower : upper + Mod - lower;
            uint32_t delta = event.subtract ? negate(value) : value;
            uint32_t answer = answers[event.query] + delta;
            answers[event.query] = answer >= Mod ? answer - Mod : answer;
        }
        return answers;
    }
};

template <class Sum = uint64_t>
class WaveletFenwickRectangleSum {
    struct Point {
        uint32_t x;
        uint32_t y;
        Sum weight;
        uint32_t id;
    };

    std::vector<Point> points;
    std::vector<uint32_t> xs;
    std::vector<uint32_t> ys;
    std::vector<uint32_t> point_positions;
    std::vector<uint64_t> rank_bits;
    std::vector<uint32_t> rank_prefix;
    std::vector<uint32_t> zero_counts;
    std::vector<Sum> fenwick;
    uint32_t size_ = 0;
    uint32_t levels = 0;
    uint32_t word_count = 0;
    uint32_t fenwick_stride = 0;

    uint32_t rank1(uint32_t level, uint32_t position) const {
        uint32_t word = position >> 6;
        uint32_t offset = position & 63;
        uint32_t result = rank_prefix[level * (word_count + 1) + word];
        if (offset)
            result +=
                std::popcount(rank_bits[level * word_count + word] & ((uint64_t(1) << offset) - 1));
        return result;
    }

    bool bit(uint32_t level, uint32_t position) const {
        return rank_bits[level * word_count + (position >> 6)] >> (position & 63) & 1;
    }

    void fenwick_add(uint32_t level, uint32_t position, Sum delta) {
        std::size_t base = std::size_t(level) * fenwick_stride;
        for (uint32_t i = position + 1; i <= size_; i += i & -i) fenwick[base + i] += delta;
    }

    Sum fenwick_prefix(uint32_t level, uint32_t end) const {
        std::size_t base = std::size_t(level) * fenwick_stride;
        Sum result{};
        for (uint32_t i = end; i; i -= i & -i) result += fenwick[base + i];
        return result;
    }

    Sum fenwick_range(uint32_t level, uint32_t left, uint32_t right) const {
        return fenwick_prefix(level, right) - fenwick_prefix(level, left);
    }

    Sum prefix_value_sum(uint32_t left, uint32_t right, uint32_t value) const {
        if (left == right || value == 0) return {};
        if (value >= ys.size()) {
            uint32_t one_left = rank1(0, left);
            uint32_t one_right = rank1(0, right);
            return fenwick_range(0, left - one_left, right - one_right) +
                   fenwick_range(0, zero_counts[0] + one_left, zero_counts[0] + one_right);
        }
        Sum result{};
        for (uint32_t level = 0; level < levels; ++level) {
            uint32_t one_left = rank1(level, left);
            uint32_t one_right = rank1(level, right);
            uint32_t zero_left = left - one_left;
            uint32_t zero_right = right - one_right;
            uint32_t shift = levels - level - 1;
            if (value >> shift & 1) {
                result += fenwick_range(level, zero_left, zero_right);
                left = zero_counts[level] + one_left;
                right = zero_counts[level] + one_right;
            } else {
                left = zero_left;
                right = zero_right;
            }
        }
        return result;
    }

  public:
    explicit WaveletFenwickRectangleSum(std::size_t point_capacity = 0) {
        points.reserve(point_capacity);
    }

    uint32_t register_point(uint32_t x, uint32_t y, Sum initial_weight = {}) {
        uint32_t id = points.size();
        points.push_back({x, y, initial_weight, id});
        return id;
    }

    void build() {
        size_ = points.size();
        if (size_ == 0) return;

        radix_sort_u32(points.begin(), points.end(), [](const Point &point) { return point.x; });
        xs.resize(size_);
        point_positions.resize(size_);
        ys.resize(size_);
        for (uint32_t i = 0; i < size_; ++i) {
            xs[i] = points[i].x;
            ys[i] = points[i].y;
            point_positions[points[i].id] = i;
        }
        radix_sort_u32(ys.begin(), ys.end(), [](uint32_t value) { return value; });
        ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

        levels = std::max<uint32_t>(1, std::bit_width(uint32_t(ys.size() - 1)));
        word_count = (size_ + 63) >> 6;
        fenwick_stride = size_ + 1;
        rank_bits.assign(std::size_t(levels) * word_count, 0);
        rank_prefix.assign(std::size_t(levels) * (word_count + 1), 0);
        zero_counts.resize(levels);
        fenwick.assign(std::size_t(levels) * fenwick_stride, Sum{});

        std::vector<uint32_t> values(size_);
        std::vector<uint32_t> next_values(size_);
        std::vector<Sum> weights(size_);
        std::vector<Sum> next_weights(size_);
        for (uint32_t i = 0; i < size_; ++i) {
            values[i] = std::lower_bound(ys.begin(), ys.end(), points[i].y) - ys.begin();
            weights[i] = points[i].weight;
        }

        for (uint32_t level = 0; level < levels; ++level) {
            uint32_t shift = levels - level - 1;
            std::size_t bit_base = std::size_t(level) * word_count;
            for (uint32_t i = 0; i < size_; ++i)
                if (values[i] >> shift & 1)
                    rank_bits[bit_base + (i >> 6)] |= uint64_t(1) << (i & 63);

            std::size_t prefix_base = std::size_t(level) * (word_count + 1);
            for (uint32_t word = 0; word < word_count; ++word)
                rank_prefix[prefix_base + word + 1] =
                    rank_prefix[prefix_base + word] + std::popcount(rank_bits[bit_base + word]);

            uint32_t zeros = size_ - rank_prefix[prefix_base + word_count];
            zero_counts[level] = zeros;
            uint32_t zero = 0;
            uint32_t one = zeros;
            for (uint32_t i = 0; i < size_; ++i) {
                uint32_t &destination = values[i] >> shift & 1 ? one : zero;
                next_values[destination] = values[i];
                next_weights[destination] = weights[i];
                ++destination;
            }

            std::size_t fenwick_base = std::size_t(level) * fenwick_stride;
            for (uint32_t i = 0; i < size_; ++i) fenwick[fenwick_base + i + 1] = next_weights[i];
            for (uint32_t i = 1; i <= size_; ++i) {
                uint32_t parent = i + (i & -i);
                if (parent <= size_) fenwick[fenwick_base + parent] += fenwick[fenwick_base + i];
            }
            values.swap(next_values);
            weights.swap(next_weights);
        }
        points.clear();
        points.shrink_to_fit();
    }

    void add(uint32_t point_id, Sum delta) {
        uint32_t position = point_positions[point_id];
        for (uint32_t level = 0; level < levels; ++level) {
            uint32_t ones = rank1(level, position);
            if (bit(level, position))
                position = zero_counts[level] + ones;
            else
                position -= ones;
            fenwick_add(level, position, delta);
        }
    }

    Sum rectangle(uint32_t left, uint32_t down, uint32_t right, uint32_t up) const {
        if (size_ == 0) return {};
        uint32_t x_left = std::lower_bound(xs.begin(), xs.end(), left) - xs.begin();
        uint32_t x_right = std::lower_bound(xs.begin(), xs.end(), right) - xs.begin();
        uint32_t y_down = std::lower_bound(ys.begin(), ys.end(), down) - ys.begin();
        uint32_t y_up = std::lower_bound(ys.begin(), ys.end(), up) - ys.begin();
        return prefix_value_sum(x_left, x_right, y_up) - prefix_value_sum(x_left, x_right, y_down);
    }
};

template <class Sum = int64_t>
class OfflineRectangleAddPointGet {
    struct Rectangle {
        uint32_t left;
        uint32_t right;
        uint32_t down;
        uint32_t up;
        Sum weight;
    };

    struct Point {
        uint32_t x;
        uint32_t y;
        uint32_t time;
    };

    struct Key {
        uint32_t coordinate;
        uint32_t id;
    };

    std::vector<Rectangle> rectangles;
    std::vector<Point> points;

    static uint32_t highest_power_of_two(uint32_t value) {
        return uint32_t(1) << (31 - std::countl_zero(value));
    }

  public:
    explicit OfflineRectangleAddPointGet(std::size_t rectangle_capacity = 0,
                                         std::size_t point_capacity = 0) {
        rectangles.reserve(rectangle_capacity);
        points.reserve(point_capacity);
    }

    void add_rectangle(uint32_t left, uint32_t down, uint32_t right, uint32_t up, Sum weight) {
        if (left == right || down == up) return;
        rectangles.push_back({left, right, down, up, weight});
    }

    void add_query(uint32_t x, uint32_t y) {
        points.push_back({x, y, uint32_t(rectangles.size())});
    }

    std::vector<Sum> solve() const {
        uint32_t rectangle_count = rectangles.size();
        std::vector<Sum> answers(points.size());
        if (rectangle_count == 0) return answers;

        std::vector<Key> x_events(2 * rectangle_count);
        std::vector<Key> y_events(2 * rectangle_count);
        for (uint32_t i = 0; i < rectangle_count; ++i) {
            x_events[2 * i] = {rectangles[i].left, 2 * i};
            x_events[2 * i + 1] = {rectangles[i].right, 2 * i + 1};
            y_events[2 * i] = {rectangles[i].down, 2 * i};
            y_events[2 * i + 1] = {rectangles[i].up, 2 * i + 1};
        }

        std::vector<std::vector<Key>> block_x(rectangle_count);
        std::vector<std::vector<Key>> block_y(rectangle_count);
        for (uint32_t i = 0; i < points.size(); ++i) {
            if (points[i].time == 0) continue;
            uint32_t end = highest_power_of_two(points[i].time);
            block_x[end - 1].push_back({points[i].x, i});
            block_y[end - 1].push_back({points[i].y, i});
        }

        auto less_coordinate = [](const Key &left, const Key &right) {
            return left.coordinate < right.coordinate;
        };
        std::vector<uint32_t> event_y_rank(2 * rectangle_count);
        std::vector<uint32_t> point_y_end(points.size());

        for (uint32_t end = 1; end <= rectangle_count; ++end) {
            uint32_t width = end & -end;
            for (uint32_t half = 1; half < width; half <<= 1) {
                auto x_right = x_events.begin() + 2 * end;
                auto x_middle = x_right - 2 * half;
                auto x_left = x_middle - 2 * half;
                std::inplace_merge(x_left, x_middle, x_right, less_coordinate);
                auto y_right = y_events.begin() + 2 * end;
                auto y_middle = y_right - 2 * half;
                auto y_left = y_middle - 2 * half;
                std::inplace_merge(y_left, y_middle, y_right, less_coordinate);
            }

            std::vector<Key> &queries_x = block_x[end - 1];
            if (queries_x.empty()) continue;
            std::vector<Key> &queries_y = block_y[end - 1];
            if (end == width) {
                radix_sort_u32(queries_x.begin(), queries_x.end(),
                               [](const Key &key) { return key.coordinate; });
                radix_sort_u32(queries_y.begin(), queries_y.end(),
                               [](const Key &key) { return key.coordinate; });
            }

            uint32_t left = end - width;
            uint32_t event_begin = 2 * left;
            uint32_t event_end = 2 * end;
            uint32_t y_count = 0;
            uint32_t previous_y = 0;
            for (uint32_t i = event_begin; i < event_end; ++i) {
                if (i == event_begin || y_events[i].coordinate != previous_y) {
                    previous_y = y_events[i].coordinate;
                    ++y_count;
                }
                event_y_rank[y_events[i].id] = y_count - 1;
            }

            uint32_t y_event = event_begin;
            uint32_t passed = 0;
            for (const Key &query : queries_y) {
                while (y_event < event_end && y_events[y_event].coordinate <= query.coordinate) {
                    uint32_t coordinate = y_events[y_event].coordinate;
                    ++passed;
                    do {
                        ++y_event;
                    } while (y_event < event_end && y_events[y_event].coordinate == coordinate);
                }
                point_y_end[query.id] = passed;
            }

            std::vector<Sum> fenwick(y_count + 1);
            auto add = [&](uint32_t position, Sum delta) {
                for (uint32_t i = position + 1; i <= y_count; i += i & -i) fenwick[i] += delta;
            };
            auto prefix_sum = [&](uint32_t prefix_end) {
                Sum result{};
                for (uint32_t i = prefix_end; i; i -= i & -i) result += fenwick[i];
                return result;
            };

            uint32_t x_event = event_begin;
            for (const Key &query : queries_x) {
                while (x_event < event_end && x_events[x_event].coordinate <= query.coordinate) {
                    uint32_t event_id = x_events[x_event++].id;
                    uint32_t rectangle = event_id >> 1;
                    Sum weight = rectangles[rectangle].weight;
                    if (event_id & 1) weight = -weight;
                    add(event_y_rank[2 * rectangle], weight);
                    add(event_y_rank[2 * rectangle + 1], -weight);
                }
                answers[query.id] += prefix_sum(point_y_end[query.id]);
            }

            for (const Key &query : queries_x) {
                uint32_t remaining = points[query.id].time - end;
                if (remaining != 0) {
                    uint32_t next = end + highest_power_of_two(remaining);
                    block_x[next - 1].push_back(query);
                }
            }
            for (const Key &query : queries_y) {
                uint32_t remaining = points[query.id].time - end;
                if (remaining != 0) {
                    uint32_t next = end + highest_power_of_two(remaining);
                    block_y[next - 1].push_back(query);
                }
            }
            queries_x.clear();
            queries_y.clear();
        }
        return answers;
    }
};

} // namespace toy
