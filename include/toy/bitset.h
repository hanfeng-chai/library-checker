#pragma once
#include <toy/buffer.h>

namespace toy {
// Higher levels mark nonempty 64-bit words. Insert/erase only propagate when
// an entire word changes between empty and nonempty.
struct Bitmap {
    std::array<Buffer<u64>, 6> levels;
    int n, height = 0;
    explicit Bitmap(int n) : n(n) {
        for (usize count = n; count; count = count > 1 ? (count + 63) / 64 : 0) {
            usize words = (count + 63) / 64; levels[height] = Buffer<u64>(words);
            std::fill(levels[height].p, levels[height].p + words, u64(0)); ++height;
            if (words == 1) break;
        }
    }
    explicit Bitmap(std::string_view initial) : Bitmap(initial.size()) {
        usize i = 0; auto one = _mm256_set1_epi8('1');
        for (; i + 64 <= initial.size(); i += 64) {
            u32 low = _mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_loadu_si256((const __m256i*)(initial.data() + i)), one));
            u32 high = _mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_loadu_si256((const __m256i*)(initial.data() + i + 32)), one));
            levels[0][i / 64] = low | (u64(high) << 32);
        }
        for (; i < initial.size(); ++i) if (initial[i] == '1') levels[0][i / 64] |= u64(1) << (i & 63);
        for (int k = 1; k < height; ++k)
            for (usize j = 0; j < levels[k - 1].n; ++j) levels[k][j / 64] |= u64(levels[k - 1][j] != 0) << (j & 63);
    }
    bool contains(u32 x) const { return (levels[0][x / 64] >> (x & 63)) & 1; }
    bool insert(u32 x) {
        bool changed = !contains(x);
        for (int k = 0; k < height; ++k, x >>= 6) {
            auto& word = levels[k][x / 64]; u64 old = word; word |= u64(1) << (x & 63); if (old) break;
        }
            return changed;
    }
    bool erase(u32 x) {
        bool changed = contains(x);
        for (int k = 0; k < height; ++k, x >>= 6)
            if (levels[k][x / 64] &= ~(u64(1) << (x & 63))) break;
            return changed;
    }
    int next(int x) const {
        if (x >= n) return -1;
        for (int k = 0; k < height; ++k, x = (x >> 6) + 1) {
            if (usize(x / 64) >= levels[k].n) return -1;
            u64 word = levels[k][x / 64] & (~u64(0) << (x & 63));
            if (word) {
                x = (x & -64) + std::countr_zero(word);
                while (k--) x = x * 64 + std::countr_zero(levels[k][x]);
                return x;
            }
        }
        return -1;
    }
    int previous(int x) const {
        x = std::min(x, n - 1);
        for (int k = 0; k < height && x >= 0; ++k, x = (x >> 6) - 1) {
            u64 word = levels[k][x / 64] & (~u64(0) >> (63 - (x & 63)));
            if (word) {
                x = (x & -64) + 63 - std::countl_zero(word);
                while (k--) x = x * 64 + 63 - std::countl_zero(levels[k][x]);
                return x;
            }
        }
        return -1;
    }
};
struct BitSet : Bitmap {
    u32 count = 0;
    alignas(32) mutable std::array<u32, 18> small{};
    mutable bool dirty = true;
    explicit BitSet(int n) : Bitmap(n) {}
    explicit BitSet(std::string_view bits) : Bitmap(bits) {
        for (u64 word : std::span<const u64>(levels[0])) count += std::popcount(word);
    }
    void insert(u32 x) { count += Bitmap::insert(x); dirty = true; }
    void erase(u32 x) { count -= Bitmap::erase(x); dirty = true; }
    [[gnu::noinline]] void refresh() const {
        std::fill(small.begin(), small.end(), u32(n)); int i = 1;
        for (int x = Bitmap::next(0); x >= 0; x = Bitmap::next(x + 1)) small[i++] = x;
        dirty = false;
    }
    // key is in [0,n). Rank among at most 16 cached keys selects either bound;
    // surrounding n sentinels represent failure, including an empty prefix.
    [[gnu::always_inline]] int bound(u32 key, bool reverse) const {
        if (!count) return -1;
        if (count <= 16) {
            if (dirty) refresh();
            auto threshold = _mm256_set1_epi32(key + reverse);
            auto a = _mm256_cmpgt_epi32(threshold, _mm256_loadu_si256((const __m256i*)(small.data() + 1)));
            auto b = _mm256_cmpgt_epi32(threshold, _mm256_loadu_si256((const __m256i*)(small.data() + 9)));
            u32 mask = _mm256_movemask_ps(_mm256_castsi256_ps(a)) | (u32(_mm256_movemask_ps(_mm256_castsi256_ps(b))) << 8);
            u32 answer = small[std::popcount(mask) + !reverse]; return answer == u32(n) ? -1 : int(answer);
        }
        u64 word = levels[0][key >> 6], left = word << (63 - (key & 63)), right = word >> (key & 63);
        u64 local = right ^ ((left ^ right) & -u64(reverse));
        if (local) {
            u32 a = key - std::countl_zero(left), b = key + std::countr_zero(right);
            return b ^ ((a ^ b) & -u32(reverse));
        }
        return reverse ? Bitmap::previous(key) : Bitmap::next(key);
    }
    int next(u32 key) const { return key >= u32(n) ? -1 : bound(key, false); }
    int previous(int key) const { return key < 0 || !n ? -1 : bound(std::min(key, n - 1), true); }
};

}
