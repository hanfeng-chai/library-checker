#pragma once

#include <bits/extc++.h>
#include <toy/csr.hpp>
#include <toy/ds.hpp>

namespace toy {

struct OfflineFrequencyEvent {
    int value;
    int first;
    int second;
    int answer;
    int delta;

    static OfflineFrequencyEvent change(int value, int position, int delta) {
        return {value, position, 0, -1, delta};
    }

    static OfflineFrequencyEvent query(
        int value, int left, int right, int answer) {
        return {value, left, right, answer, 0};
    }
};

inline std::vector<int> offline_point_value_frequencies(
    int value_count, std::span<const int> initial_values,
    std::span<const int> final_values,
    std::vector<OfflineFrequencyEvent> events, int answer_count) {
    struct Position {
        int value;
        int index;
    };

    std::vector<Position> initial_positions;
    std::vector<Position> final_positions;
    initial_positions.reserve(initial_values.size());
    final_positions.reserve(final_values.size());
    for (int i = 0; i < (int)initial_values.size(); ++i) {
        initial_positions.push_back({initial_values[i], i});
        final_positions.push_back({final_values[i], i});
    }
    CsrBuckets initial(value_count, std::move(initial_positions),
                       [](const Position& item) { return item.value; });
    CsrBuckets final(value_count, std::move(final_positions),
                     [](const Position& item) { return item.value; });
    CsrBuckets grouped_events(
        value_count, std::move(events),
        [](const OfflineFrequencyEvent& event) { return event.value; });

    FenwickBitset frequencies((int)initial_values.size());
    std::vector<int> answers(answer_count);
    for (int value = 0; value < value_count; ++value) {
        std::span<const OfflineFrequencyEvent> value_events =
            grouped_events[value];
        bool queried = std::ranges::any_of(
            value_events,
            [](const OfflineFrequencyEvent& event) { return !event.delta; });
        if (!queried) continue;
        for (const Position& item : initial[value])
            frequencies.set(item.index, true);
        for (const OfflineFrequencyEvent& event : value_events) {
            if (event.delta)
                frequencies.set(event.first, event.delta > 0);
            else
                answers[event.answer] =
                    frequencies.count(event.first, event.second);
        }
        for (const Position& item : final[value])
            frequencies.set(item.index, false);
    }
    return answers;
}

} // namespace toy
