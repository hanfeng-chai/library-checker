#pragma once

#include <bits/extc++.h>

namespace toy {

class ModeCounter {
    std::vector<int> frequency, position;
    std::vector<std::vector<int>> buckets;
    int maximum = 0;
    void erase_from(int value, int count) {
        auto &bucket = buckets[count];
        int at = position[value], last = bucket.back();
        bucket[at] = last;
        position[last] = at;
        bucket.pop_back();
    }
    void insert_into(int value, int count) {
        position[value] = buckets[count].size();
        buckets[count].push_back(value);
    }

  public:
    ModeCounter(int values, int length) : frequency(values), position(values), buckets(length + 1) {
        buckets[0].reserve(values);
        for (int value = 0; value < values; ++value) insert_into(value, 0);
    }
    void add(int value) {
        erase_from(value, frequency[value]);
        insert_into(value, ++frequency[value]);
        maximum = std::max(maximum, frequency[value]);
    }
    void remove(int value) {
        erase_from(value, frequency[value]);
        insert_into(value, --frequency[value]);
        while (maximum && buckets[maximum].empty()) --maximum;
    }
    std::pair<int, int> mode() const { return {buckets[maximum].back(), maximum}; }
};

template <class T>
class StaticRangeMode {
    struct Mode {
        int value = 0;
        int count = 0;
    };

    int length;
    int block_size;
    int block_count;
    std::vector<T> values;
    std::vector<int> ids;
    std::vector<int> grouped_positions;
    std::vector<int> group_start;
    std::vector<int> position_in_group;
    std::vector<Mode> block_modes;

  public:
    explicit StaticRangeMode(const std::vector<T> &input)
        : length(input.size()), block_size(1), block_count(0), ids(length),
          position_in_group(length) {
        std::vector<std::pair<T, int>> ordered(length);
        for (int i = 0; i < length; ++i) ordered[i] = {input[i], i};
        std::sort(ordered.begin(), ordered.end());
        for (const auto &[value, index] : ordered) {
            if (values.empty() || values.back() != value) values.push_back(value);
            ids[index] = values.size() - 1;
        }
        unsigned root = std::max(1U, (unsigned)std::sqrt(std::max(length, 1)));
        block_size = values.size() <= 64 ? std::bit_ceil(root) : std::bit_floor(root);
        block_count = (length + block_size - 1) / block_size;
        block_modes.resize(block_count * block_count);

        group_start.assign(values.size() + 1, 0);
        for (int id : ids) ++group_start[id + 1];
        std::partial_sum(group_start.begin(), group_start.end(), group_start.begin());
        std::vector<int> cursor = group_start;
        grouped_positions.resize(length);
        for (int i = 0; i < length; ++i) {
            int position = cursor[ids[i]]++;
            grouped_positions[position] = i;
            position_in_group[i] = position;
        }

        std::vector<int> frequency(values.size());
        for (int left_block = 0; left_block < block_count; ++left_block) {
            std::fill(frequency.begin(), frequency.end(), 0);
            Mode mode;
            for (int right_block = left_block; right_block < block_count; ++right_block) {
                int begin = right_block * block_size;
                int end = std::min(begin + block_size, length);
                for (int i = begin; i < end; ++i) {
                    int count = ++frequency[ids[i]];
                    if (count > mode.count) mode = {ids[i], count};
                }
                block_modes[left_block * block_count + right_block] = mode;
            }
        }
    }

    std::pair<T, int> query(int left, int right) const {
        assert(0 <= left && left < right && right <= length);
        int full_left = (left + block_size - 1) / block_size;
        int full_right = right / block_size;
        Mode mode;
        if (full_left < full_right) mode = block_modes[full_left * block_count + (full_right - 1)];

        int left_end = std::min(full_left * block_size, right);
        for (int i = left; i < left_end; ++i) {
            int id = ids[i];
            int position = position_in_group[i];
            if (position + mode.count < group_start[id + 1] &&
                grouped_positions[position + mode.count] < right) {
                int count = mode.count;
                do {
                    ++count;
                } while (position + count < group_start[id + 1] &&
                         grouped_positions[position + count] < right);
                mode = {id, count};
                if (2 * mode.count >= right - left) return {values[mode.value], mode.count};
            }
        }

        int right_begin = std::max(full_right * block_size, left_end);
        for (int i = right; i-- > right_begin;) {
            int id = ids[i];
            int position = position_in_group[i];
            if (mode.count <= position - group_start[id] &&
                grouped_positions[position - mode.count] >= left) {
                int count = mode.count;
                do {
                    ++count;
                } while (count <= position - group_start[id] &&
                         grouped_positions[position - count] >= left);
                mode = {id, count};
                if (2 * mode.count >= right - left) return {values[mode.value], mode.count};
            }
        }
        return {values[mode.value], mode.count};
    }
};

} // namespace toy
