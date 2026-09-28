#pragma once

#include <bits/extc++.h>
#include <toy/sort.hpp>

namespace toy {

template<unsigned Bits>
class WaveletMatrix {
    int length;
    std::array<std::vector<int>, Bits> prefix_ones;
    std::array<int, Bits> zero_count{};

public:
    explicit WaveletMatrix(std::vector<uint32_t> values) : length(values.size()) {
        std::vector<uint32_t> buffer(length);
        for (unsigned level = 0; level < Bits; ++level) {
            int bit = Bits - 1 - level;
            auto& prefix = prefix_ones[level];
            prefix.resize(length + 1);
            for (int i = 0; i < length; ++i)
                prefix[i + 1] = prefix[i] + (values[i] >> bit & 1);
            zero_count[level] = length - prefix[length];
            int zero = 0;
            int one = zero_count[level];
            for (uint32_t value : values)
                buffer[(value >> bit & 1) ? one++ : zero++] = value;
            values.swap(buffer);
        }
    }

    uint32_t kth(int left, int right, int index) const {
        uint32_t result = 0;
        for (unsigned level = 0; level < Bits; ++level) {
            int left_ones = prefix_ones[level][left];
            int right_ones = prefix_ones[level][right];
            int zeros = (right - left) - (right_ones - left_ones);
            if (index < zeros) {
                left -= left_ones;
                right -= right_ones;
            } else {
                result |= 1U << (Bits - 1 - level);
                index -= zeros;
                left = zero_count[level] + left_ones;
                right = zero_count[level] + right_ones;
            }
        }
        return result;
    }

    int frequency(int left, int right, uint32_t value) const {
        for (unsigned level = 0; level < Bits; ++level) {
            int left_ones = prefix_ones[level][left];
            int right_ones = prefix_ones[level][right];
            if (value >> (Bits - 1 - level) & 1) {
                left = zero_count[level] + left_ones;
                right = zero_count[level] + right_ones;
            } else {
                left -= left_ones;
                right -= right_ones;
            }
        }
        return right - left;
    }
};

class CompressedWaveletMatrix {
    struct Block {
        uint64_t bits = 0;
        uint32_t before = 0;
        uint32_t padding = 0;
    };

    int length;
    int levels;
    int stride;
    std::vector<uint32_t> original_values;
    std::vector<int> zero_count;
    std::vector<Block> blocks;

    int rank_one(int level, int position) const {
        const Block& block = blocks[level * stride + (position >> 6)];
        unsigned offset = position & 63;
        uint64_t mask = offset ? (uint64_t(1) << offset) - 1 : 0;
        return block.before + std::popcount(block.bits & mask);
    }

public:
    explicit CompressedWaveletMatrix(const std::vector<uint32_t>& input)
        : length(input.size()) {
        assert(length > 0);
        std::vector<std::pair<uint32_t, int>> ordered(length);
        for (int i = 0; i < length; ++i) ordered[i] = {input[i], i};
        radix_sort_u32<30, 15>(
            ordered.begin(), ordered.end(),
            [](const auto& item) { return item.first; });

        std::vector<uint32_t> current(length), buffer(length), ones_buffer(length);
        for (const auto& [value, index] : ordered) {
            if (original_values.empty() || original_values.back() != value)
                original_values.push_back(value);
            current[index] = original_values.size() - 1;
        }
        levels = original_values.size() <= 1
            ? 1 : std::bit_width((unsigned)(original_values.size() - 1));
        stride = (length + 63) / 64 + 1;
        zero_count.resize(levels);
        blocks.resize((std::size_t)levels * stride);

        for (int level = 0; level < levels; ++level) {
            int bit = levels - 1 - level;
            Block* row = blocks.data() + (std::size_t)level * stride;
            uint32_t ones = 0;
            int zeros = 0;
            int one_values = 0;
            int word = 0;
            int offset = 0;
            uint64_t word_bits = 0;
            for (uint32_t value : current) {
                uint32_t one = value >> bit & 1;
                word_bits |= uint64_t(one) << offset;
                buffer[zeros] = value;
                ones_buffer[one_values] = value;
                zeros += one ^ 1;
                one_values += one;
                if (++offset == 64) {
                    row[word].before = ones;
                    row[word].bits = word_bits;
                    ones += std::popcount(word_bits);
                    ++word;
                    offset = 0;
                    word_bits = 0;
                }
            }
            if (offset) {
                row[word].before = ones;
                row[word].bits = word_bits;
                ones += std::popcount(word_bits);
            }
            row[stride - 1].before = ones;
            zero_count[level] = zeros;
            std::copy_n(ones_buffer.begin(), one_values,
                        buffer.begin() + zeros);
            current.swap(buffer);
        }
    }

    std::vector<uint32_t> kth_batch(
        std::vector<int> left, std::vector<int> right,
        std::vector<int> index) const {
        assert(left.size() == right.size() && left.size() == index.size());
        std::vector<uint32_t> answer(left.size());
        for (int level = 0; level < levels; ++level) {
            uint32_t bit = uint32_t(1) << (levels - 1 - level);
            const Block* row =
                blocks.data() + (std::size_t)level * stride;
            int level_zeros = zero_count[level];
            for (std::size_t query = 0; query < left.size(); ++query) {
                int left_position = left[query];
                int right_position = right[query];
                const Block& left_block = row[left_position >> 6];
                const Block& right_block = row[right_position >> 6];
                int left_offset = left_position & 63;
                int right_offset = right_position & 63;
                int left_ones = left_block.before + std::popcount(
                    left_block.bits &
                    ((uint64_t(1) << left_offset) - 1));
                int right_ones = right_block.before + std::popcount(
                    right_block.bits &
                    ((uint64_t(1) << right_offset) - 1));
                int left_zeros = left_position - left_ones;
                int right_zeros = right_position - right_ones;
                int zeros = right_zeros - left_zeros;
                int take_one = -(index[query] >= zeros);
                left[query] =
                    (left_zeros & ~take_one) |
                    ((level_zeros + left_ones) & take_one);
                right[query] =
                    (right_zeros & ~take_one) |
                    ((level_zeros + right_ones) & take_one);
                index[query] -= zeros & take_one;
                answer[query] |= bit & take_one;
            }
        }
        for (uint32_t& value : answer) value = original_values[value];
        return answer;
    }
};

template<class T>
class MergeSortTree {
    int size;
    std::vector<std::vector<T>> tree;
    std::vector<T> values;

    int count_less_equal(int left, int right, T value) const {
        int result = 0;
        for (left += size, right += size; left < right; left >>= 1, right >>= 1) {
            if (left & 1) {
                result += std::upper_bound(tree[left].begin(), tree[left].end(), value) -
                          tree[left].begin();
                ++left;
            }
            if (right & 1) {
                --right;
                result += std::upper_bound(tree[right].begin(), tree[right].end(), value) -
                          tree[right].begin();
            }
        }
        return result;
    }

public:
    explicit MergeSortTree(const std::vector<T>& input)
        : size(std::bit_ceil((unsigned)input.size())), tree(2 * size), values(input) {
        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());
        for (int i = 0; i < (int)input.size(); ++i) tree[size + i].push_back(input[i]);
        for (int node = size - 1; node; --node)
            std::merge(tree[node * 2].begin(), tree[node * 2].end(),
                       tree[node * 2 + 1].begin(), tree[node * 2 + 1].end(),
                       std::back_inserter(tree[node]));
    }

    T kth(int left, int right, int index) const {
        int low = 0;
        int high = values.size();
        while (low < high) {
            int middle = (low + high) / 2;
            if (count_less_equal(left, right, values[middle]) > index) high = middle;
            else low = middle + 1;
        }
        return values[low];
    }

    int count_less(int left, int right, T value) const {
        int result = 0;
        for (left += size, right += size; left < right; left >>= 1, right >>= 1) {
            if (left & 1) {
                result += std::lower_bound(tree[left].begin(), tree[left].end(), value) -
                          tree[left].begin();
                ++left;
            }
            if (right & 1) {
                --right;
                result += std::lower_bound(tree[right].begin(), tree[right].end(), value) -
                          tree[right].begin();
            }
        }
        return result;
    }
};

} // namespace toy
