#pragma once
#include <toy/bitset.h>
namespace toy {
// 512-key bitmap leaves; a singleton stays directly in the directory.
template <unsigned Bits = 30>
struct XorSet {
    static_assert(Bits >= 9 && Bits <= 32);
    static constexpr u32 singleton = 1u << 31;
    struct alignas(64) Leaf {
        u64 word[8];
    };
    Buffer<u32> directory;
    Buffer<Leaf> leaves;
    Bitmap occupied;
    u32 free = 0, blocks = 0, count = 0, xor_all = 0;
    explicit XorSet(usize reserve = 0)
        : directory(usize(1) << (Bits - 9)), leaves(1, reserve + 1), occupied(directory.n) {
        std::fill(directory.p, directory.p + directory.n, 0u);
    }
    static u32 choose(u64 word, u32 key) {
        // Keep the preferred half whenever it is nonempty, from high bit to low.
        if ((word >> key) & 1) return key;
        if (std::has_single_bit(word)) return std::countr_zero(word);
        u32 result = 0;
        [&]<usize... K>(std::index_sequence<K...>) {
            (([&] {
                 constexpr unsigned step = 32 >> K;
                 u32 side = key & step;
                 u64 part = (word >> side) & ((u64(1) << step) - 1);
                 if (!part) side ^= step;
                 word = (word >> side) & ((u64(1) << step) - 1);
                 result |= side;
             }()),
             ...);
        }(std::make_index_sequence<6>{});
        return result;
    }
    u32 nearest_block(u32 key) const {
        int level = 0;
        u32 index = key;
        u64 word = 0;
        if (blocks <= 64) {
            level = occupied.height - 1;
            index = key >> (6 * level);
            word = occupied.levels[level][index >> 6];
        } else
            for (;; ++level, index >>= 6)
                if ((word = occupied.levels[level][index >> 6])) break;
        u32 result = (index & -64u) | choose(word, index & 63);
        while (level--)
            result =
                64 * result + choose(occupied.levels[level][result], (key >> (6 * level)) & 63);
        return result;
    }
    u32 allocate() {
        if (free) {
            u32 i = free;
            free = leaves[i].word[0];
            leaves[i] = {};
            return i;
        }
        if (leaves.n == leaves.capacity) leaves.reserve(std::max<usize>(4, 2 * leaves.capacity));
        leaves[leaves.n] = {};
        return leaves.n++;
    }
    u32 size() const { return count; }
    bool contains(u32 key) const {
        u32 entry = directory[key >> 9];
        if (!entry) return false;
        if (entry & singleton) return (entry ^ singleton) == (key & 511);
        return leaves[entry].word[(key >> 6) & 7] >> (key & 63) & 1;
    }
    bool insert(u32 key) {
        auto &entry = directory[key >> 9];
        if (!entry) {
            entry = singleton | (key & 511);
            occupied.insert(key >> 9);
            ++blocks;
        } else {
            if (entry & singleton) {
                u32 old = entry ^ singleton;
                if (old == (key & 511)) return false;
                entry = allocate();
                leaves[entry].word[old >> 6] = u64(1) << (old & 63);
            }
            auto &word = leaves[entry].word[(key >> 6) & 7];
            u64 bit = u64(1) << (key & 63);
            if (word & bit) return false;
            word |= bit;
        }
        ++count;
        xor_all ^= key;
        return true;
    }
    bool erase(u32 key) {
        auto &entry = directory[key >> 9];
        if (!entry) return false;
        if (entry & singleton) {
            if ((entry ^ singleton) != (key & 511)) return false;
            entry = 0;
        } else {
            auto &leaf = leaves[entry];
            auto &word = leaf.word[(key >> 6) & 7];
            u64 bit = u64(1) << (key & 63);
            if (!(word & bit)) return false;
            word ^= bit;
            auto a = _mm256_load_si256((const __m256i *)leaf.word),
                 b = _mm256_load_si256((const __m256i *)(leaf.word + 4));
            a = _mm256_or_si256(a, b);
            if (_mm256_testz_si256(a, a)) {
                leaf.word[0] = free;
                free = entry;
                entry = 0;
            }
        }
        if (!entry) {
            occupied.erase(key >> 9);
            --blocks;
        }
        --count;
        xor_all ^= key;
        return true;
    }
    u32 min_xor(u32 key) const {
        if (count == 1) return xor_all ^ key;
        u32 prefix = key >> 9;
        if (blocks <= 64 || !directory[prefix]) prefix = nearest_block(prefix);
        u32 entry = directory[prefix];
        if (entry & singleton) return ((prefix << 9) | (entry ^ singleton)) ^ key;
        const auto &leaf = leaves[entry];
        u32 preferred = (key >> 6) & 7;
        for (u32 distance = 0;; ++distance) {
            u32 index = preferred ^ distance;
            if (u64 word = leaf.word[index])
                return ((prefix << 9) | (index << 6) | choose(word, key & 63)) ^ key;
        }
    }
};
} // namespace toy
