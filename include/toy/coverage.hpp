#pragma once

#include <bits/extc++.h>

namespace toy {

struct SortedUniqueCoordinatesTag {};
inline constexpr SortedUniqueCoordinatesTag sorted_unique_coordinates;

template<class Coordinate = int, class Length = int64_t>
class CoveredLengthTree {
    struct Node {
        Length child_sum = 0;
        Length span = 0;
        uint32_t cover = 0;

        Length covered() const { return cover ? span : child_sum; }
    };

    int segment_count;
    int capacity;
    int height;
    std::vector<Coordinate> coordinates;
    std::vector<Node> tree;

    void initialize() {
        segment_count = std::max<int>(coordinates.size() - 1, 0);
        capacity = std::bit_ceil(
            (unsigned)std::max(segment_count, 1));
        height = std::countr_zero((unsigned)capacity);
        tree.resize(2 * capacity);
        for (int i = 0; i < segment_count; ++i)
            tree[capacity + i].span =
                (Length)(coordinates[i + 1] - coordinates[i]);
        for (int node = capacity - 1; node; --node)
            tree[node].span =
                tree[node * 2].span + tree[node * 2 + 1].span;
    }

    void pull(int node) {
        tree[node].child_sum =
            tree[node * 2].covered() + tree[node * 2 + 1].covered();
    }

public:
    explicit CoveredLengthTree(std::vector<Coordinate> values)
        : coordinates(std::move(values)) {
        std::sort(coordinates.begin(), coordinates.end());
        coordinates.erase(
            std::unique(coordinates.begin(), coordinates.end()),
            coordinates.end());
        initialize();
    }

    CoveredLengthTree(
        std::vector<Coordinate> values, SortedUniqueCoordinatesTag)
        : coordinates(std::move(values)) {
        initialize();
    }

    int index(Coordinate value) const {
        return std::lower_bound(
                   coordinates.begin(), coordinates.end(), value) -
               coordinates.begin();
    }

    void update(int left, int right, int delta) {
        if (left >= right) return;
        int first_leaf = left + capacity;
        int last_leaf = right - 1 + capacity;
        for (int first = first_leaf, last = right + capacity;
             first < last; first >>= 1, last >>= 1) {
            if (first & 1) {
                if (delta > 0) ++tree[first].cover;
                else --tree[first].cover;
                ++first;
            }
            if (last & 1) {
                --last;
                if (delta > 0) ++tree[last].cover;
                else --tree[last].cover;
            }
        }
        for (int level = 1; level <= height; ++level) {
            int first = first_leaf >> level;
            int last = last_leaf >> level;
            pull(first);
            if (last != first) pull(last);
        }
    }

    Length covered() const {
        return segment_count ? tree[1].covered() : Length{};
    }
};

} // namespace toy
