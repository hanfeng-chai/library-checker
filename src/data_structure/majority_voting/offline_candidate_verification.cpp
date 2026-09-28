#include <toy/io.hpp>
#include <toy/offline_frequency.hpp>
#include <toy/range.hpp>
#include <toy/sort.hpp>

namespace {
struct Vote {
    int value = 0;
    int balance = 0;
};

struct MergeVote {
    Vote operator()(Vote left, Vote right) const {
        if (!left.balance) return right;
        if (!right.balance) return left;
        if (left.value == right.value)
            return {left.value, left.balance + right.balance};
        if (left.balance > right.balance)
            return {left.value, left.balance - right.balance};
        return {right.value, right.balance - left.balance};
    }
};

struct Query {
    toy::u32 type;
    int first;
    int second;
    int slot = -1;
};
}

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    std::vector<std::pair<toy::u32, int>> tagged;
    tagged.reserve(n + query_count);
    int slots = 0;
    for (int i = 0; i < n; ++i)
        tagged.push_back({input.read_uniform<10, toy::u32>(), slots++});

    std::vector<Query> queries(query_count);
    for (Query& query : queries) {
        query.type = input.read_fixed<1, toy::u32>();
        query.first = input.read_uniform<6, toy::u32>();
        query.second = input.read_uniform<10, toy::u32>();
        if (query.type == 0)
            tagged.push_back({(toy::u32)query.second, query.slot = slots++});
    }

    toy::radix_sort_u32<30, 15>(
        tagged.begin(), tagged.end(),
        [](const auto& item) { return item.first; });
    std::vector<toy::u32> values;
    std::vector<int> compressed(slots);
    for (const auto& [value, slot] : tagged) {
        if (values.empty() || values.back() != value) values.push_back(value);
        compressed[slot] = values.size() - 1;
    }

    std::vector<int> initial(compressed.begin(), compressed.begin() + n);
    std::vector<int> current = initial;
    std::vector<Vote> leaves(n);
    for (int i = 0; i < n; ++i) leaves[i] = {initial[i], 1};
    toy::SegmentTree candidates(leaves, Vote{}, MergeVote{});

    std::vector<toy::OfflineFrequencyEvent> events;
    events.reserve(2 * query_count);
    std::vector<int> candidate_ids;
    std::vector<int> lengths;
    int update_slot = n;
    for (const Query& query : queries) {
        if (query.type == 0) {
            int value = compressed[update_slot++];
            events.push_back(toy::OfflineFrequencyEvent::change(
                current[query.first], query.first, -1));
            current[query.first] = value;
            events.push_back(toy::OfflineFrequencyEvent::change(
                value, query.first, 1));
            candidates.set(query.first, {value, 1});
        } else {
            int candidate =
                candidates.fold(query.first, query.second).value;
            int answer = candidate_ids.size();
            candidate_ids.push_back(candidate);
            lengths.push_back(query.second - query.first);
            events.push_back(toy::OfflineFrequencyEvent::query(
                candidate, query.first, query.second, answer));
        }
    }

    std::vector<int> frequencies = toy::offline_point_value_frequencies(
        values.size(), initial, current, std::move(events),
        candidate_ids.size());
    for (int i = 0; i < (int)frequencies.size(); ++i) {
        if (2 * frequencies[i] > lengths[i])
            output.writeln((toy::u64)values[candidate_ids[i]]);
        else
            output.write("-1\n");
    }
}
