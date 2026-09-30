#pragma once
#include <toy/hash.h>

namespace toy {
// Group positions by value without sorting the keys. Positions within a group
// remain increasing, so two binary searches answer a frequency query.
struct StaticFrequency {
    HashMap<u32, u32> ids;
    Buffer<u32> offsets, positions;
    explicit StaticFrequency(span<const u32> a) : ids(a.size()), offsets(a.size() + 2), positions(a.size()) {
        fill(offsets.p, offsets.p + offsets.n, 0u); u32 groups = 0;
        for (u32 x : a) { auto& id = ids[x]; if (!id) id = ++groups; ++offsets[id + 1]; }
        for (u32 i = 1; i <= groups + 1; ++i) offsets[i] += offsets[i - 1];
        Buffer<u32> cursor(groups + 1); memcpy(cursor.p, offsets.p, cursor.n * 4);
        for (u32 i = 0; i < a.size(); ++i) positions[cursor[ids.get(a[i])]++] = i;
    }
    u32 count(u32 l, u32 r, u32 value) const {
        u32 id = ids.get(value); if (!id) return 0;
        const auto* first = positions.p + offsets[id]; const auto* last = positions.p + offsets[id + 1];
        if (last - first <= 8) { u32 sum = 0; for (; first != last; ++first) sum += l <= *first && *first < r; return sum; }
        return lower_bound(first, last, r) - lower_bound(first, last, l);
    }
};
struct FrequencyQuery { u32 left, right, value; };
inline Buffer<u32> static_frequencies(span<const u32> a, span<const FrequencyQuery> queries) {
    Buffer<u32> offsets(a.size() + 2), answer(queries.size());
    fill(offsets.p, offsets.p + offsets.n, 0u); fill(answer.p, answer.p + answer.n, 0u);
    for (auto q : queries) { ++offsets[q.left + 1]; ++offsets[q.right + 1]; }
    for (usize i = 1; i < offsets.n; ++i) offsets[i] += offsets[i - 1];
    struct Endpoint { u32 key, tag; };
    Buffer<Endpoint> events(2 * queries.size()); Buffer<u32> cursor(a.size() + 1);
    memcpy(cursor.p, offsets.p, cursor.n * 4);
    for (u32 i = 0; i < queries.size(); ++i) {
        auto q = queries[i]; events[cursor[q.left]++] = {q.value, 2 * i}; events[cursor[q.right]++] = {q.value, 2 * i + 1};
    }
    HashMap<u32, u32> counts(a.size());
    for (usize i = 0; i <= a.size(); ++i) {
        for (u32 j = offsets[i]; j < offsets[i + 1]; ++j) {
            auto e = events[j]; u32 value = counts.get(e.key);
            answer[e.tag >> 1] += (e.tag & 1) ? value : -value;
        }
        if (i < a.size()) ++counts[a[i]];
    }
    return answer;
}

}
