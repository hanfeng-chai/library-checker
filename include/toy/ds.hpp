#pragma once

#include <bits/extc++.h>
#include <immintrin.h>

namespace toy {

class DisjointSetUnion {
    std::vector<int> parent_or_size;

public:
    explicit DisjointSetUnion(int n) : parent_or_size(n, -1) {}

    int leader(int x) {
        int root = x;
        while (parent_or_size[root] >= 0) root = parent_or_size[root];
        while (x != root) {
            int parent = parent_or_size[x];
            parent_or_size[x] = root;
            x = parent;
        }
        return root;
    }

    bool merge(int a, int b) {
        a = leader(a);
        b = leader(b);
        if (a == b) return false;
        if (parent_or_size[a] > parent_or_size[b]) std::swap(a, b);
        parent_or_size[a] += parent_or_size[b];
        parent_or_size[b] = a;
        return true;
    }

    bool same(int a, int b) { return leader(a) == leader(b); }
    int size(int x) { return -parent_or_size[leader(x)]; }
};

class RollbackUnionFind {
    struct Change {
        int parent;
        int child;
        int child_size;
    };

    std::vector<int> parent_or_size;
    std::vector<Change> history;

public:
    explicit RollbackUnionFind(int n, std::size_t capacity = 0)
        : parent_or_size(n, -1) {
        history.reserve(capacity);
    }

    int leader(int vertex) const {
        while (parent_or_size[vertex] >= 0)
            vertex = parent_or_size[vertex];
        return vertex;
    }

    bool same(int first, int second) const {
        return leader(first) == leader(second);
    }

    bool merge(int first, int second) {
        first = leader(first);
        second = leader(second);
        if (first == second) {
            history.push_back({-1, -1, 0});
            return false;
        }
        if (parent_or_size[first] > parent_or_size[second])
            std::swap(first, second);
        history.push_back({first, second, parent_or_size[second]});
        parent_or_size[first] += parent_or_size[second];
        parent_or_size[second] = first;
        return true;
    }

    void undo() {
        Change change = history.back();
        history.pop_back();
        if (change.parent < 0) return;
        parent_or_size[change.parent] -= change.child_size;
        parent_or_size[change.child] = change.child_size;
    }

    std::size_t snapshot() const { return history.size(); }

    void rollback(std::size_t state) {
        while (history.size() > state) undo();
    }
};

template<class T>
class FenwickTree {
    std::vector<T> data;

public:
    explicit FenwickTree(int n) : data(n + 1) {}

    template<class Range>
    explicit FenwickTree(const Range& values) : data(values.size() + 1) {
        for (int i = 0; i < (int)values.size(); ++i) data[i + 1] += values[i];
        for (int i = 1; i < (int)data.size(); ++i) {
            int parent = i + (i & -i);
            if (parent < (int)data.size()) data[parent] += data[i];
        }
    }

    void add(int index, T delta) {
        for (++index; index < (int)data.size(); index += index & -index)
            data[index] += delta;
    }

    T prefix_sum(int end) const {
        T result{};
        for (; end; end -= end & -end) result += data[end];
        return result;
    }

    T sum(int left, int right) const {
        return prefix_sum(right) - prefix_sum(left);
    }

    int lower_bound(T target) const {
        if (target <= T{}) return 0;
        int index = 0;
        for (int step = std::bit_floor((unsigned)data.size()); step; step >>= 1) {
            int next = index + step;
            if (next < (int)data.size() && data[next] < target) {
                index = next;
                target -= data[next];
            }
        }
        return index;
    }
};

class FenwickBitset {
    std::vector<uint64_t> bits;
    std::vector<int> block_counts;

    static uint64_t low_bits(unsigned width) {
        return (uint64_t{1} << width) - 1;
    }

    int prefix_blocks(int end) const {
        int result = 0;
        for (; end; end -= end & -end) result += block_counts[end - 1];
        return result;
    }

public:
    explicit FenwickBitset(int size)
        : bits((size + 63) / 64 + 1),
          block_counts((size + 63) / 64) {}

    bool contains(int index) const {
        return bits[index >> 6] >> (index & 63) & 1;
    }

    void set(int index, bool value) {
        int block = index >> 6;
        uint64_t mask = uint64_t{1} << (index & 63);
        if (bool(bits[block] & mask) == value) return;
        bits[block] ^= mask;
        int delta = value ? 1 : -1;
        for (++block; block <= (int)block_counts.size();
             block += block & -block)
            block_counts[block - 1] += delta;
    }

    int count(int left, int right) const {
        int left_block = left >> 6;
        int right_block = right >> 6;
        int result =
            std::popcount(bits[right_block] & low_bits(right & 63)) -
            std::popcount(bits[left_block] & low_bits(left & 63));
        return result + prefix_blocks(right_block) -
               prefix_blocks(left_block);
    }
};

template<class T, std::size_t Capacity>
class FixedCenteredDeque {
    std::array<T, 2 * Capacity + 1> data{};
    std::size_t left = Capacity;
    std::size_t right = Capacity;

public:
    bool empty() const { return left == right; }
    std::size_t size() const { return right - left; }

    void push_front(const T& value) { data[--left] = value; }
    void push_back(const T& value) { data[right++] = value; }
    void pop_front() { ++left; }
    void pop_back() { --right; }

    T& operator[](std::size_t index) { return data[left + index]; }
    const T& operator[](std::size_t index) const {
        return data[left + index];
    }
};

class PredecessorSet {
    std::vector<std::vector<uint64_t>> levels;
    int universe;

    static int next_in_word(const std::vector<uint64_t>& words, int index) {
        int word = index >> 6;
        if (word >= (int)words.size()) return -1;
        uint64_t candidates = words[word] & (~0ULL << (index & 63));
        if (candidates) return word * 64 + std::countr_zero(candidates);
        return -1;
    }

    static int previous_in_word(const std::vector<uint64_t>& words, int index) {
        if (index < 0) return -1;
        int word = std::min<int>(index >> 6, words.size() - 1);
        uint64_t mask = (index & 63) == 63 ? ~0ULL : (1ULL << ((index & 63) + 1)) - 1;
        uint64_t candidates = words[word] & mask;
        if (candidates) return word * 64 + 63 - std::countl_zero(candidates);
        return -1;
    }

public:
    explicit PredecessorSet(int n) : universe(n) {
        for (int size = n; ; size = (size + 63) / 64) {
            levels.emplace_back((size + 63) / 64);
            if (size <= 64) break;
        }
    }

    void assign(std::string_view bits) {
        for (auto& level : levels) std::fill(level.begin(), level.end(), 0);
        int index = 0;
        for (; index + 64 <= universe; index += 64) {
            __m256i ones = _mm256_set1_epi8('1');
            uint32_t low = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                _mm256_loadu_si256((const __m256i*)(bits.data() + index)), ones));
            uint32_t high = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                _mm256_loadu_si256((const __m256i*)(bits.data() + index + 32)), ones));
            levels[0][index / 64] = low | (uint64_t)high << 32;
        }
        for (; index < universe; ++index)
            if (bits[index] == '1') levels[0][index / 64] |= 1ULL << (index % 64);
        for (int level = 1; level < (int)levels.size(); ++level)
            for (int child = 0; child < (int)levels[level - 1].size(); ++child)
                if (levels[level - 1][child])
                    levels[level][child / 64] |= 1ULL << (child % 64);
    }

    bool contains(int x) const {
        return levels[0][x >> 6] >> (x & 63) & 1;
    }

    void insert(int x) {
        for (auto& level : levels) {
            uint64_t& word = level[x >> 6];
            uint64_t bit = 1ULL << (x & 63);
            if (word & bit) break;
            word |= bit;
            x >>= 6;
        }
    }

    void erase(int x) {
        for (auto& level : levels) {
            uint64_t& word = level[x >> 6];
            word &= ~(1ULL << (x & 63));
            if (word) break;
            x >>= 6;
        }
    }

    int next(int x) const {
        if (x >= universe) return -1;
        int position = x;
        for (int level = 0; level < (int)levels.size(); ++level) {
            int found = next_in_word(levels[level], position);
            if (found >= 0) {
                while (level--) {
                    uint64_t word = levels[level][found];
                    found = found * 64 + std::countr_zero(word);
                }
                return found < universe ? found : -1;
            }
            position = (position >> 6) + 1;
        }
        return -1;
    }

    int previous(int x) const {
        if (universe == 0 || x < 0) return -1;
        int position = std::min(x, universe - 1);
        for (int level = 0; level < (int)levels.size(); ++level) {
            int found = previous_in_word(levels[level], position);
            if (found >= 0) {
                while (level--) {
                    uint64_t word = levels[level][found];
                    found = found * 64 + 63 - std::countl_zero(word);
                }
                return found;
            }
            position = (position >> 6) - 1;
        }
        return -1;
    }
};

template <std::size_t MaxUniverse>
class BoundedPredecessorSet {
        static_assert(MaxUniverse <= (1ULL << 24));
        static constexpr std::size_t leaf_count = (MaxUniverse + 63) / 64;
        static constexpr std::size_t level1_count = (leaf_count + 63) / 64;
        static constexpr std::size_t level2_count = (level1_count + 63) / 64;

        std::array<uint64_t, leaf_count> leaf{};
        std::array<uint64_t, level1_count> level1{};
        std::array<uint64_t, level2_count> level2{};
        uint64_t root = 0;
        std::size_t assigned_leaves = 0;

    public:
        void assign(std::string_view bits) {
            level1.fill(0);
            level2.fill(0);
            root = 0;
            std::size_t index = 0;
            const __m256i ones = _mm256_set1_epi8('1');
            for (; index + 64 <= bits.size(); index += 64) {
                uint32_t low = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                    _mm256_loadu_si256((const __m256i*)(bits.data() + index)), ones));
                uint32_t high = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
                    _mm256_loadu_si256((const __m256i*)(bits.data() + index + 32)), ones));
                leaf[index / 64] = low | (uint64_t)high << 32;
            }
            if (index < bits.size()) {
                leaf[index / 64] = 0;
                for (; index < bits.size(); ++index)
                    if (bits[index] == '1') leaf[index / 64] |= 1ULL << (index % 64);
            }
            std::size_t used_leaves = (bits.size() + 63) / 64;
            if (used_leaves < assigned_leaves)
                std::fill(leaf.begin() + used_leaves, leaf.begin() + assigned_leaves, 0);
            assigned_leaves = used_leaves;
            for (std::size_t i = 0; i < used_leaves; ++i)
                level1[i / 64] |= uint64_t(leaf[i] != 0) << (i % 64);
            for (std::size_t i = 0; i < (leaf_count + 63) / 64; ++i)
                level2[i / 64] |= uint64_t(level1[i] != 0) << (i % 64);
            for (std::size_t i = 0; i < (level1_count + 63) / 64; ++i)
                root |= uint64_t(level2[i] != 0) << i;
        }

        void insert(unsigned x) {
            leaf[x >> 6] |= 1ULL << (x & 63);
            level1[x >> 12] |= 1ULL << ((x >> 6) & 63);
            level2[x >> 18] |= 1ULL << ((x >> 12) & 63);
            root |= 1ULL << (x >> 18);
        }

        void erase(unsigned x) {
            if (!(leaf[x >> 6] &= ~(1ULL << (x & 63))))
                if (!(level1[x >> 12] &= ~(1ULL << ((x >> 6) & 63))))
                    if (!(level2[x >> 18] &= ~(1ULL << ((x >> 12) & 63))))
                        root &= ~(1ULL << (x >> 18));
        }

        bool contains(unsigned x) const {
            return leaf[x >> 6] >> (x & 63) & 1;
        }

        int successor(unsigned x) const {
            uint64_t word = leaf[x >> 6] & (~0ULL << (x & 63));
            if (word) return int((x >> 6) << 6 | std::countr_zero(word));
            word = level1[x >> 12] & (-2ULL << ((x >> 6) & 63));
            if (word) {
                unsigned answer = (x >> 12) << 6 | std::countr_zero(word);
                return int(answer << 6 | std::countr_zero(leaf[answer]));
            }
            word = level2[x >> 18] & (-2ULL << ((x >> 12) & 63));
            if (word) {
                unsigned answer = (x >> 18) << 6 | std::countr_zero(word);
                answer = answer << 6 | std::countr_zero(level1[answer]);
                return int(answer << 6 | std::countr_zero(leaf[answer]));
            }
            word = root & (-2ULL << (x >> 18));
            if (!word) return -1;
            unsigned answer = std::countr_zero(word);
            answer = answer << 6 | std::countr_zero(level2[answer]);
            answer = answer << 6 | std::countr_zero(level1[answer]);
            return int(answer << 6 | std::countr_zero(leaf[answer]));
        }

        int predecessor(unsigned x) const {
            uint64_t word = leaf[x >> 6] & ~(-2ULL << (x & 63));
            if (word) return int((x >> 6) << 6 | (63 - std::countl_zero(word)));
            word = level1[x >> 12] & ~(~0ULL << ((x >> 6) & 63));
            if (word) {
                unsigned answer = (x >> 12) << 6 | (63 - std::countl_zero(word));
                return int(answer << 6 | (63 - std::countl_zero(leaf[answer])));
            }
            word = level2[x >> 18] & ~(~0ULL << ((x >> 12) & 63));
            if (word) {
                unsigned answer = (x >> 18) << 6 | (63 - std::countl_zero(word));
                answer = answer << 6 | (63 - std::countl_zero(level1[answer]));
                return int(answer << 6 | (63 - std::countl_zero(leaf[answer])));
            }
            word = root & ~(~0ULL << (x >> 18));
            if (!word) return -1;
            unsigned answer = 63 - std::countl_zero(word);
            answer = answer << 6 | (63 - std::countl_zero(level2[answer]));
            answer = answer << 6 | (63 - std::countl_zero(level1[answer]));
            return int(answer << 6 | (63 - std::countl_zero(leaf[answer])));
        }
};

} // namespace toy
