#include <toy/hash.hpp>
#include <toy/io.hpp>
#include <toy/offline_frequency.hpp>

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    int query_count = input.read_uniform<6, toy::u32>();
    toy::U32HashMap ids(n + query_count);
    int value_count = 0;
    auto get_or_create = [&](toy::u32 value) {
        int id = ids.get(value);
        if (!id) {
            id = ++value_count;
            ids.set(value, id);
        }
        return id - 1;
    };

    std::vector<int> initial(n);
    for (int& value : initial)
        value = get_or_create(input.read_uniform<10, toy::u32>());
    if (query_count && n == 0) input.skip_spaces();

    std::vector<int> current = initial;
    std::vector<toy::OfflineFrequencyEvent> events;
    events.reserve(2 * query_count);
    int answer_count = 0;
    for (int query = 0; query < query_count; ++query) {
        toy::u32 type = input.read_fixed<1, toy::u32>();
        int first = input.read_uniform<6, toy::u32>();
        if (type == 0) {
            int value =
                get_or_create(input.read_uniform<10, toy::u32>());
            events.push_back(toy::OfflineFrequencyEvent::change(
                current[first], first, -1));
            current[first] = value;
            events.push_back(toy::OfflineFrequencyEvent::change(
                value, first, 1));
        } else {
            int right = input.read_uniform<6, toy::u32>();
            toy::u32 raw_value = input.read_uniform<10, toy::u32>();
            int value = ids.get(raw_value);
            if (value)
                events.push_back(toy::OfflineFrequencyEvent::query(
                    value - 1, first, right, answer_count));
            ++answer_count;
        }
    }

    for (int answer : toy::offline_point_value_frequencies(
             value_count, initial, current, std::move(events), answer_count))
        output.write_padded_u32(answer);
}
