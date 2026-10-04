#pragma once
#include <toy/buckets.h>
#include <toy/prefix_tree.h>
namespace toy {
struct FrequencyEvent {
    u32 value, left, right, answer;
    static FrequencyEvent change(u32 value, u32 position, bool present) {
        return {value, position, 0, present ? ~1u : ~0u};
    }
    static FrequencyEvent query(u32 value, u32 l, u32 r, u32 id) { return {value, l, r, id}; }
};
// Process each value's chronological events on one reusable rank bitmap.
inline Buffer<u32> point_value_frequencies(u32 value_count, std::span<const u32> initial,
                                           std::span<const u32> final,
                                           Buffer<FrequencyEvent> events, u32 answer_count) {
    struct Position {
        u32 value, index;
    };
    Buffer<Position> first(initial.size()), last(final.size());
    for (u32 i = 0; i < initial.size(); ++i) {
        first[i] = {initial[i], i};
        last[i] = {final[i], i};
    }
    Buckets starts(value_count, std::move(first), [](auto x) { return x.value; }),
        ends(value_count, std::move(last), [](auto x) { return x.value; });
    Buckets grouped(value_count, std::move(events), [](auto x) { return x.value; });
    Buffer<u64> bits(initial.size() / 64 + 1);
    std::fill(bits.p, bits.p + bits.n, u64(0));
    PrefixTree32 counts(bits.n);
    auto set = [&](u32 i, bool present) {
        auto &word = bits[i / 64];
        u64 bit = u64(1) << (i & 63);
        if (bool(word & bit) == present) return;
        word ^= bit;
        counts.add(i / 64, present ? 1 : -1);
    };
    auto prefix = [&](u32 i) {
        return counts.prefix(i / 64) + std::popcount(bits[i / 64] & ((u64(1) << (i & 63)) - 1));
    };
    Buffer<u32> answer(answer_count);
    std::fill(answer.p, answer.p + answer.n, 0u);
    for (u32 value = 0; value < value_count; ++value) {
        auto group = grouped[value];
        bool queried = false;
        for (auto x : group) queried |= x.answer < answer_count;
        if (!queried) continue;
        for (auto x : starts[value]) set(x.index, true);
        for (auto x : group)
            if (x.answer < answer_count)
                answer[x.answer] = prefix(x.right) - prefix(x.left);
            else
                set(x.left, x.answer == ~1u);
        for (auto x : ends[value]) set(x.index, false);
    }
    return answer;
}
} // namespace toy
