#include <toy/io.h>
#include <toy/coverage.h>
#include <toy/radix_sort.h>
using namespace toy;
int main() {
    Reader in; Writer out; u32 n = in.read<u32, 6>(); if (!n) { out.write(0); return 0; }
    struct Event { u32 x, first, second; }; struct Endpoint { u32 value, slot; };
    Buffer<Event> events(2 * n); Buffer<Endpoint> endpoints(2 * n);
    for (u32 i = 0; i < n; ++i) {
        u32 l = in.read<u32, 10>(), d = in.read<u32, 10>(), r = in.read<u32, 10>(), u = in.read<u32, 10>();
        events[2 * i] = {l, 0, 0}; events[2 * i + 1] = {r, 0, 0};
        endpoints[2 * i] = {d, 2 * i}; endpoints[2 * i + 1] = {u, 2 * i + 1};
    }
    radix_sort(std::span(endpoints.p, endpoints.n), [](Endpoint e) { return e.value; });
    Buffer<u32> coordinates(2 * n); usize count = 0;
    for (usize i = 0; i < endpoints.n; ++i) {
        auto e = endpoints[i]; if (!i || e.value != endpoints[i - 1].value) coordinates[count++] = e.value;
        // Reversing the two y endpoints encodes a removal event.
        events[e.slot].first = count - 1; events[e.slot ^ 1].second = count - 1;
    }
    endpoints = {}; coordinates.n = count;
    radix_sort(std::span(events.p, events.n), [](Event e) { return e.x; });
    CoverageTree cover{std::span<const u32>(coordinates)}; coordinates = {};
    u64 area = 0; u32 previous = events[0].x;
    for (auto e : std::span(events.p, events.n)) {
        area += u64(e.x - previous) * cover.covered(); previous = e.x;
        if (e.first < e.second) cover.add(e.first, e.second, 1); else cover.add(e.second, e.first, -1);
    }
    out.write(area);
}
