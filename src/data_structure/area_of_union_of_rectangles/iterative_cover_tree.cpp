#include <toy/coverage.hpp>
#include <toy/io.hpp>

#include <toy/sort.hpp>

struct Event {
    toy::u32 x;
    toy::u32 first_y;
    toy::u32 second_y;
};

struct Endpoint {
    toy::u32 coordinate;
    toy::u32 event;
};

int main() {
    toy::Reader input(toy::direct_mapping);
    toy::Writer output;
    int n = input.read_uniform<6, toy::u32>();
    std::vector<Event> events;
    std::vector<Endpoint> endpoints;
    events.reserve(2 * n);
    endpoints.reserve(2 * n);
    for (int i = 0; i < n; ++i) {
        toy::u32 left = input.read_uniform<10, toy::u32>();
        toy::u32 down = input.read_uniform<10, toy::u32>();
        toy::u32 right = input.read_uniform<10, toy::u32>();
        toy::u32 up = input.read_uniform<10, toy::u32>();
        events.push_back({left, 0, 0});
        events.push_back({right, 0, 0});
        endpoints.push_back({down, (toy::u32)(2 * i)});
        endpoints.push_back({up, (toy::u32)(2 * i + 1)});
    }

    toy::radix_sort_u32(endpoints.begin(), endpoints.end(),
                        [](const Endpoint &endpoint) { return endpoint.coordinate; });
    std::vector<toy::u32> coordinates;
    coordinates.reserve(endpoints.size());
    for (const Endpoint &endpoint : endpoints) {
        if (coordinates.empty() || coordinates.back() != endpoint.coordinate)
            coordinates.push_back(endpoint.coordinate);
        toy::u32 index = coordinates.size() - 1;
        events[endpoint.event].first_y = index;
        events[endpoint.event ^ 1].second_y = index;
    }
    toy::radix_sort_u32(events.begin(), events.end(), [](const Event &event) { return event.x; });

    toy::CoveredLengthTree<toy::u32, toy::u32> tree(std::move(coordinates),
                                                    toy::sorted_unique_coordinates);
    toy::u64 area = 0;
    toy::u32 previous = events.front().x;
    for (const Event &event : events) {
        area += (toy::u64)(event.x - previous) * tree.covered();
        previous = event.x;
        if (event.first_y < event.second_y)
            tree.update(event.first_y, event.second_y, 1);
        else
            tree.update(event.second_y, event.first_y, -1);
    }
    output.writeln(area);
}
