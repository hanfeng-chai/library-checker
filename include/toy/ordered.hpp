#pragma once

#include <bits/extc++.h>
#include <toy/ds.hpp>

namespace toy {

template<unsigned Bits>
class BinaryTrieSet {
    struct Node {
        int child[2]{};
        int count = 0;
    };
    std::vector<Node> nodes{{}};

public:
    explicit BinaryTrieSet(std::size_t capacity = 0) {
        nodes.reserve(1 + capacity * (Bits + 1));
    }

    bool contains(uint32_t value) const {
        int node = 0;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            node = nodes[node].child[value >> bit & 1];
            if (!node || !nodes[node].count) return false;
        }
        return true;
    }

    bool insert(uint32_t value) {
        if (contains(value)) return false;
        int node = 0;
        ++nodes[node].count;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            int direction = value >> bit & 1;
            if (!nodes[node].child[direction]) {
                nodes[node].child[direction] = nodes.size();
                nodes.push_back({});
            }
            node = nodes[node].child[direction];
            ++nodes[node].count;
        }
        return true;
    }

    bool erase(uint32_t value) {
        if (!contains(value)) return false;
        int node = 0;
        --nodes[node].count;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            node = nodes[node].child[value >> bit & 1];
            --nodes[node].count;
        }
        return true;
    }

    uint32_t min_xor(uint32_t value) const {
        int node = 0;
        uint32_t result = 0;
        for (int bit = Bits - 1; bit >= 0; --bit) {
            int preferred = value >> bit & 1;
            int next = nodes[node].child[preferred];
            if (!next || !nodes[next].count) {
                preferred ^= 1;
                result |= 1U << bit;
                next = nodes[node].child[preferred];
            }
            node = next;
        }
        return result;
    }
};

template<unsigned Bits>
class PatriciaTrieSet {
    static_assert(Bits < 32);
    struct Node {
        uint32_t value = 0;
        int child[2]{};
        int bit = -1;
    };

    std::vector<Node> nodes{{}};
    int root = 0;

    int make_leaf(uint32_t value) {
        nodes.push_back({value, {}, -1});
        return nodes.size() - 1;
    }

public:
    explicit PatriciaTrieSet(std::size_t capacity = 0) {
        nodes.reserve(1 + 2 * capacity);
    }

    bool contains(uint32_t value) const {
        int node = root;
        if (!node) return false;
        while (nodes[node].bit >= 0)
            node = nodes[node].child[value >> nodes[node].bit & 1];
        return nodes[node].value == value;
    }

    bool insert(uint32_t value) {
        assert(value < (uint32_t(1) << Bits));
        if (!root) {
            root = make_leaf(value);
            return true;
        }
        int leaf = root;
        while (nodes[leaf].bit >= 0)
            leaf = nodes[leaf].child[value >> nodes[leaf].bit & 1];
        uint32_t difference = value ^ nodes[leaf].value;
        if (!difference) return false;
        int bit = std::bit_width(difference) - 1;

        int* link = &root;
        int node = root;
        while (nodes[node].bit > bit) {
            link = &nodes[node].child[value >> nodes[node].bit & 1];
            node = *link;
        }
        int new_leaf = make_leaf(value);
        int direction = value >> bit & 1;
        Node branch;
        branch.bit = bit;
        branch.child[direction] = new_leaf;
        branch.child[direction ^ 1] = node;
        nodes.push_back(branch);
        *link = nodes.size() - 1;
        return true;
    }

    bool erase(uint32_t value) {
        if (!root) return false;
        int grandparent = 0;
        int parent = 0;
        int node = root;
        int grandparent_direction = 0;
        int parent_direction = 0;
        while (nodes[node].bit >= 0) {
            grandparent = parent;
            grandparent_direction = parent_direction;
            parent = node;
            parent_direction = value >> nodes[node].bit & 1;
            node = nodes[node].child[parent_direction];
        }
        if (nodes[node].value != value) return false;
        if (!parent) {
            root = 0;
        } else {
            int sibling = nodes[parent].child[parent_direction ^ 1];
            if (!grandparent) root = sibling;
            else nodes[grandparent].child[grandparent_direction] = sibling;
        }
        return true;
    }

    uint32_t min_xor(uint32_t value) const {
        assert(root);
        int node = root;
        while (nodes[node].bit >= 0)
            node = nodes[node].child[value >> nodes[node].bit & 1];
        return value ^ nodes[node].value;
    }
};

class ChunkedXorTrieSet {
    static constexpr int low_bits = 9;
    static constexpr int group_count = 1 << (30 - low_bits);
    static constexpr uint32_t inline_flag = uint32_t(1) << 31;

    struct Leaf {
        uint64_t words[8]{};
        uint8_t nonempty_words = 0;

        void insert(uint32_t value) {
            int word = value >> 6;
            words[word] |= uint64_t(1) << (value & 63);
            nonempty_words |= uint8_t(1) << word;
        }

        bool erase(uint32_t value) {
            int word = value >> 6;
            uint64_t mask = uint64_t(1) << (value & 63);
            if (!(words[word] & mask)) return false;
            words[word] ^= mask;
            if (!words[word]) nonempty_words ^= uint8_t(1) << word;
            return true;
        }
    };

    uint64_t top_mask = 0;
    std::array<uint64_t, 8> level1{};
    std::array<uint64_t, 8 * 64> level2{};
    std::array<uint64_t, 8 * 64 * 64> level3{};
    std::vector<uint32_t> leaf_indices;
    std::vector<Leaf> leaves;

    static uint32_t xor_min_bit(uint64_t mask, uint32_t desired) {
        constexpr uint64_t one_masks[] = {
            0xaaaaaaaaaaaaaaaaULL,
            0xccccccccccccccccULL,
            0xf0f0f0f0f0f0f0f0ULL,
            0xff00ff00ff00ff00ULL,
            0xffff0000ffff0000ULL,
            0xffffffff00000000ULL,
        };
        for (int bit = 5; bit >= 0; --bit) {
            uint64_t preferred = desired >> bit & 1
                ? mask & one_masks[bit] : mask & ~one_masks[bit];
            if (preferred) mask = preferred;
        }
        return std::countr_zero(mask);
    }

    void mark_group(uint32_t value) {
        top_mask |= uint64_t(1) << (value >> 27);
        level1[value >> 27] |= uint64_t(1) << (value >> 21 & 63);
        level2[value >> 21] |= uint64_t(1) << (value >> 15 & 63);
        level3[value >> 15] |= uint64_t(1) << (value >> 9 & 63);
    }

    void unmark_group(uint32_t value) {
        uint64_t mask = uint64_t(1) << (value >> 9 & 63);
        uint64_t& at3 = level3[value >> 15];
        at3 ^= mask;
        if (at3) return;

        mask = uint64_t(1) << (value >> 15 & 63);
        uint64_t& at2 = level2[value >> 21];
        at2 ^= mask;
        if (at2) return;

        mask = uint64_t(1) << (value >> 21 & 63);
        uint64_t& at1 = level1[value >> 27];
        at1 ^= mask;
        if (at1) return;
        top_mask ^= uint64_t(1) << (value >> 27);
    }

public:
    explicit ChunkedXorTrieSet(std::size_t capacity = 0)
        : leaf_indices(group_count), leaves(1) {
        leaves.reserve(capacity / 2 + 1);
    }

    bool contains(uint32_t value) const {
        uint32_t entry = leaf_indices[value >> low_bits];
        if (!entry) return false;
        uint32_t low = value & 511;
        if (entry & inline_flag) return (entry ^ inline_flag) == low;
        return leaves[entry].words[low >> 6] >> (low & 63) & 1;
    }

    bool insert(uint32_t value) {
        uint32_t low = value & 511;
        uint32_t& entry = leaf_indices[value >> low_bits];
        if (!entry) {
            entry = inline_flag | low;
            mark_group(value);
            return true;
        }
        if (entry & inline_flag) {
            uint32_t previous = entry ^ inline_flag;
            if (previous == low) return false;
            entry = leaves.size();
            leaves.emplace_back();
            leaves.back().insert(previous);
            leaves.back().insert(low);
            return true;
        }
        Leaf& leaf = leaves[entry];
        uint64_t mask = uint64_t(1) << (low & 63);
        if (leaf.words[low >> 6] & mask) return false;
        leaf.insert(low);
        return true;
    }

    bool erase(uint32_t value) {
        uint32_t low = value & 511;
        uint32_t& entry = leaf_indices[value >> low_bits];
        if (!entry) return false;
        if (entry & inline_flag) {
            if ((entry ^ inline_flag) != low) return false;
            entry = 0;
            unmark_group(value);
            return true;
        }
        Leaf& leaf = leaves[entry];
        if (!leaf.erase(low)) return false;
        if (leaf.nonempty_words) return true;
        entry = 0;
        unmark_group(value);
        return true;
    }

    uint32_t min_xor(uint32_t value) const {
        uint32_t result = xor_min_bit(top_mask, value >> 27);
        result = result << 6 | xor_min_bit(
            level1[result], value >> 21 & 63);
        result = result << 6 | xor_min_bit(
            level2[result], value >> 15 & 63);
        result = result << 6 | xor_min_bit(
            level3[result], value >> 9 & 63);

        uint32_t entry = leaf_indices[result];
        result <<= low_bits;
        if (entry & inline_flag)
            return (result | (entry ^ inline_flag)) ^ value;
        const Leaf& leaf = leaves[entry];
        uint32_t word = xor_min_bit(
            leaf.nonempty_words, value >> 6 & 7);
        uint32_t bit = xor_min_bit(
            leaf.words[word], value & 63);
        return (result | word << 6 | bit) ^ value;
    }
};

class BitBlockOrderedSet {
    std::vector<uint64_t> bits;
    FenwickTree<int> counts;
    int element_count = 0;

public:
    explicit BitBlockOrderedSet(int universe)
        : bits((universe + 63) / 64), counts((int)bits.size()) {}

    int size() const { return element_count; }

    bool contains(int value) const {
        return bits[value >> 6] >> (value & 63) & 1;
    }

    bool insert(int value) {
        uint64_t mask = uint64_t(1) << (value & 63);
        uint64_t& word = bits[value >> 6];
        if (word & mask) return false;
        word |= mask;
        counts.add(value >> 6, 1);
        ++element_count;
        return true;
    }

    bool erase(int value) {
        uint64_t mask = uint64_t(1) << (value & 63);
        uint64_t& word = bits[value >> 6];
        if (!(word & mask)) return false;
        word ^= mask;
        counts.add(value >> 6, -1);
        --element_count;
        return true;
    }

    int rank(int end) const {
        int block = end >> 6;
        int result = counts.prefix_sum(block);
        if ((end & 63) && block < (int)bits.size())
            result += std::popcount(
                bits[block] & ((uint64_t(1) << (end & 63)) - 1));
        return result;
    }

    int kth(int index) const {
        int block = counts.lower_bound(index + 1);
        int within = index - counts.prefix_sum(block);
        uint64_t selected = _pdep_u64(uint64_t(1) << within, bits[block]);
        return block * 64 + std::countr_zero(selected);
    }
};

class WidePrefixTree {
    static constexpr int branch_bits = 5;
    static constexpr int branch = 1 << branch_bits;

    struct alignas(32) Block {
        int prefix[branch]{};
    };

    struct MaskTable {
        alignas(32) int values[branch][branch]{};

        constexpr MaskTable() {
            for (int child = 0; child < branch; ++child)
                for (int i = child + 1; i < branch; ++i)
                    values[child][i] = -1;
        }
    };

    std::vector<std::vector<Block>> levels;

    static const MaskTable& masks() {
        static constexpr MaskTable table;
        return table;
    }

    static int select_child(const Block& block, int index) {
        __m256i target = _mm256_set1_epi32(index + 1);
        int count = 0;
        for (int offset = 0; offset < branch; offset += 8) {
            __m256i prefix = _mm256_load_si256(
                (const __m256i*)(block.prefix + offset));
            __m256i before = _mm256_cmpgt_epi32(target, prefix);
            count += std::popcount((unsigned)_mm256_movemask_ps(
                _mm256_castsi256_ps(before)));
        }
        return count - 1;
    }

public:
    explicit WidePrefixTree(std::vector<int> counts) {
        if (counts.empty()) counts.push_back(0);
        while (true) {
            int block_count = (counts.size() + branch - 1) / branch;
            std::vector<Block> current(block_count);
            std::vector<int> parent(block_count);
            for (int block = 0; block < block_count; ++block) {
                int total = 0;
                for (int child = 0; child < branch; ++child) {
                    current[block].prefix[child] = total;
                    int index = block * branch + child;
                    if (index < (int)counts.size()) total += counts[index];
                }
                parent[block] = total;
            }
            levels.push_back(std::move(current));
            if (block_count == 1) break;
            counts = std::move(parent);
        }
    }

    int prefix_sum(int end) const {
        int result = 0;
        for (const auto& level : levels) {
            int block = end >> branch_bits;
            if (block < (int)level.size())
                result += level[block].prefix[end & (branch - 1)];
            end = block;
        }
        return result;
    }

    void add(int index, int delta) {
        __m256i change = _mm256_set1_epi32(delta);
        for (auto& level : levels) {
            int block = index >> branch_bits;
            int child = index & (branch - 1);
            for (int offset = 0; offset < branch; offset += 8) {
                __m256i value = _mm256_load_si256(
                    (__m256i*)(level[block].prefix + offset));
                __m256i mask = _mm256_load_si256(
                    (const __m256i*)(masks().values[child] + offset));
                value = _mm256_add_epi32(
                    value, _mm256_and_si256(change, mask));
                _mm256_store_si256(
                    (__m256i*)(level[block].prefix + offset), value);
            }
            index = block;
        }
    }

    std::pair<int, int> kth(int index) const {
        int block = 0;
        for (int level = levels.size(); level--;) {
            const Block& current = levels[level][block];
            int child = select_child(current, index);
            index -= current.prefix[child];
            block = block * branch + child;
        }
        return {block, index};
    }
};

class WideBitBlockOrderedSet {
    std::vector<uint64_t> bits;
    int element_count = 0;
    WidePrefixTree counts;

public:
    WideBitBlockOrderedSet(int universe, std::span<const int> initial)
        : bits((universe + 63) / 64),
          counts([&] {
              std::vector<int> word_counts(bits.size());
              for (int value : initial) {
                  uint64_t mask = uint64_t(1) << (value & 63);
                  uint64_t& word = bits[value >> 6];
                  if (!(word & mask)) {
                      word |= mask;
                      ++element_count;
                  }
              }
              for (int i = 0; i < (int)bits.size(); ++i)
                  word_counts[i] = std::popcount(bits[i]);
              return word_counts;
          }()) {}

    int size() const { return element_count; }

    bool contains(int value) const {
        return bits[value >> 6] >> (value & 63) & 1;
    }

    bool insert(int value) {
        uint64_t mask = uint64_t(1) << (value & 63);
        uint64_t& word = bits[value >> 6];
        if (word & mask) return false;
        word |= mask;
        counts.add(value >> 6, 1);
        ++element_count;
        return true;
    }

    bool erase(int value) {
        uint64_t mask = uint64_t(1) << (value & 63);
        uint64_t& word = bits[value >> 6];
        if (!(word & mask)) return false;
        word ^= mask;
        counts.add(value >> 6, -1);
        --element_count;
        return true;
    }

    int rank(int end) const {
        int block = end >> 6;
        int result = counts.prefix_sum(block);
        if ((end & 63) && block < (int)bits.size())
            result += std::popcount(
                bits[block] & ((uint64_t(1) << (end & 63)) - 1));
        return result;
    }

    int kth(int index) const {
        auto [block, within] = counts.kth(index);
        uint64_t selected =
            _pdep_u64(uint64_t(1) << within, bits[block]);
        return block * 64 + std::countr_zero(selected);
    }
};

template<class T>
class DoubleEndedPriorityQueue {
    std::priority_queue<T, std::vector<T>, std::greater<T>> minimum;
    std::priority_queue<T> maximum;
    std::priority_queue<T, std::vector<T>, std::greater<T>> removed_minimum;
    std::priority_queue<T> removed_maximum;

    void clean_minimum() {
        while (!removed_minimum.empty() && minimum.top() == removed_minimum.top()) {
            minimum.pop();
            removed_minimum.pop();
        }
    }

    void clean_maximum() {
        while (!removed_maximum.empty() && maximum.top() == removed_maximum.top()) {
            maximum.pop();
            removed_maximum.pop();
        }
    }

public:
    template<class Range>
    explicit DoubleEndedPriorityQueue(const Range& values) {
        for (T value : values) push(value);
    }

    void push(T value) {
        minimum.push(value);
        maximum.push(value);
    }

    T pop_min() {
        clean_minimum();
        T value = minimum.top();
        minimum.pop();
        removed_maximum.push(value);
        return value;
    }

    T pop_max() {
        clean_maximum();
        T value = maximum.top();
        maximum.pop();
        removed_minimum.push(value);
        return value;
    }
};

template<class T, class Compare = std::less<T>>
class MinMaxHeap {
    std::vector<T> data;
    [[no_unique_address]] Compare compare;
    std::size_t maximum_index = 0;

    bool is_min_level(std::size_t index) const {
        return std::bit_width(index + 1) & 1;
    }

    bool less(const T& left, const T& right) const {
        return compare(left, right);
    }

    void refresh_maximum() {
        if (data.size() <= 2) maximum_index = data.size() - 1;
        else maximum_index = less(data[1], data[2]) ? 2 : 1;
    }

    void bubble_min(std::size_t index, T value) {
        while (index >= 3) {
            std::size_t grandparent = (index - 3) / 4;
            if (!less(value, data[grandparent])) break;
            data[index] = std::move(data[grandparent]);
            index = grandparent;
        }
        data[index] = std::move(value);
    }

    void bubble_max(std::size_t index, T value) {
        while (index >= 3) {
            std::size_t grandparent = (index - 3) / 4;
            if (!less(data[grandparent], value)) break;
            data[index] = std::move(data[grandparent]);
            index = grandparent;
        }
        data[index] = std::move(value);
    }

    template<bool Minimum>
    bool better(const T& left, const T& right) const {
        if constexpr (Minimum) return less(left, right);
        else return less(right, left);
    }

    template<bool Minimum>
    void push_down(std::size_t index) {
        T value = std::move(data[index]);
        while (true) {
            std::size_t first_child = 2 * index + 1;
            if (first_child >= data.size()) break;
            std::size_t first_grandchild = 4 * index + 3;

            if (first_grandchild + 3 < data.size()) {
                std::size_t left = first_grandchild +
                    better<Minimum>(data[first_grandchild + 1],
                                    data[first_grandchild]);
                std::size_t right = first_grandchild + 2 +
                    better<Minimum>(data[first_grandchild + 3],
                                    data[first_grandchild + 2]);
                std::size_t best = better<Minimum>(data[right], data[left])
                    ? right : left;
                if (!better<Minimum>(data[best], value)) break;
                data[index] = std::move(data[best]);
                std::size_t parent = (best - 1) / 2;
                if constexpr (Minimum) {
                    if (less(data[parent], value)) {
                        T displaced = std::move(data[parent]);
                        data[parent] = std::move(value);
                        value = std::move(displaced);
                    }
                } else {
                    if (less(value, data[parent])) {
                        T displaced = std::move(data[parent]);
                        data[parent] = std::move(value);
                        value = std::move(displaced);
                    }
                }
                index = best;
                continue;
            }

            std::size_t best = first_child;
            std::size_t end =
                std::min(data.size(), first_grandchild + 4);
            if (first_child + 1 < data.size() &&
                better<Minimum>(data[first_child + 1], data[best]))
                best = first_child + 1;
            for (std::size_t candidate = first_grandchild;
                 candidate < end; ++candidate)
                if (better<Minimum>(data[candidate], data[best]))
                    best = candidate;
            if (better<Minimum>(data[best], value)) {
                data[index] = std::move(data[best]);
                if (best >= first_grandchild) {
                    std::size_t parent = (best - 1) / 2;
                    if constexpr (Minimum) {
                        if (less(data[parent], value))
                            std::swap(data[parent], value);
                    } else {
                        if (less(value, data[parent]))
                            std::swap(data[parent], value);
                    }
                }
                data[best] = std::move(value);
                return;
            }
            break;
        }
        data[index] = std::move(value);
    }

public:
    MinMaxHeap() = default;

    template<class Range>
    explicit MinMaxHeap(const Range& values) : data(values.begin(), values.end()) {
        for (std::size_t index = data.size() / 2; index--;) {
            if (is_min_level(index)) push_down<true>(index);
            else push_down<false>(index);
        }
        if (!data.empty()) refresh_maximum();
    }

    void reserve(std::size_t capacity) { data.reserve(capacity); }

    const T& min() const { return data.front(); }

    const T& max() const {
        return data[maximum_index];
    }

    void push(T value) {
        data.push_back(value);
        std::size_t index = data.size() - 1;
        if (!index) {
            maximum_index = 0;
            return;
        }
        std::size_t parent = (index - 1) / 2;
        T inserted = std::move(data[index]);
        if (is_min_level(index)) {
            if (less(data[parent], inserted)) {
                data[index] = std::move(data[parent]);
                bubble_max(parent, std::move(inserted));
            } else {
                bubble_min(index, std::move(inserted));
            }
        } else {
            if (less(inserted, data[parent])) {
                data[index] = std::move(data[parent]);
                bubble_min(parent, std::move(inserted));
            } else {
                bubble_max(index, std::move(inserted));
            }
        }
        refresh_maximum();
    }

    T pop_min() {
        T result = data.front();
        if (data.size() == 1) {
            data.pop_back();
            maximum_index = 0;
            return result;
        }
        data.front() = std::move(data.back());
        data.pop_back();
        push_down<true>(0);
        refresh_maximum();
        return result;
    }

    T pop_max() {
        std::size_t index = maximum_index;
        T result = data[index];
        if (index == data.size() - 1) {
            data.pop_back();
            if (!data.empty()) refresh_maximum();
            else maximum_index = 0;
            return result;
        }
        data[index] = std::move(data.back());
        data.pop_back();
        push_down<false>(index);
        refresh_maximum();
        return result;
    }
};

} // namespace toy
