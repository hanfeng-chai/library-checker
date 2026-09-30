#pragma once
#include <toy/prefix_tree.h>
#include <toy/bitset.h>

namespace toy {
// Membership is packed in 64-bit words; a prefix tree counts bits per word.
struct OrderedSet {
    Buffer<u64> bits;
    PrefixTree32 counts;
    Bitmap nonempty;
    u32 universe;
    OrderedSet(usize universe, span<const u32> initial = {}) : bits(universe / 64 + 1), nonempty(bits.n), universe(universe) {
        fill(bits.p, bits.p + bits.n, u64(0));
        for (u32 x : initial) bits[x / 64] |= u64(1) << (x & 63);
        Buffer<u32> weights(bits.n, bits.n + 1);
        for (usize i = 0; i < bits.n; ++i) { weights[i] = popcount(bits[i]); if (bits[i]) nonempty.insert(i); }
        counts = PrefixTree32(std::move(weights));
    }
    u32 size() const { return counts.total; }
    bool contains(u32 x) const { return bits[x / 64] >> (x & 63) & 1; }
    bool set(u32 x, bool present) {
        auto& word = bits[x / 64]; u64 bit = u64(1) << (x & 63);
        if (bool(word & bit) == present) return false;
        u64 old = word; word ^= bit;
        if (!old) nonempty.insert(x / 64); else if (!word) nonempty.erase(x / 64);
        counts.add(x / 64, present ? 1 : -1); return true;
    }
    bool insert(u32 x) { return set(x, true); }
    bool erase(u32 x) { return set(x, false); }
    u32 rank(u32 end) const { return counts.prefix(end / 64) + popcount(bits[end / 64] & ((u64(1) << (end & 63)) - 1)); }
    int next(u32 x) const {
        if (x >= universe) return -1;
        u64 word = bits[x / 64] & (~u64(0) << (x & 63));
        if (word) return (x & -64u) + countr_zero(word);
        int block = nonempty.next(x / 64 + 1);
        return block < 0 ? -1 : 64 * block + countr_zero(bits[block]);
    }
    int previous(int x) const {
        if (x < 0 || !universe) return -1; x = min<u32>(x, universe - 1);
        u64 word = bits[x / 64] & (~u64(0) >> (63 - (x & 63)));
        if (word) return (x & -64) + 63 - countl_zero(word);
        int block = nonempty.previous(x / 64 - 1);
        return block < 0 ? -1 : 64 * block + 63 - countl_zero(bits[block]);
    }
    u32 kth(u32 rank) const {
        auto [block, within] = counts.select(rank);
        // BMI2 deposits one bit in the requested position among the set bits.
        return 64 * block + countr_zero(_pdep_u64(u64(1) << within, bits[block]));
    }
};
}
