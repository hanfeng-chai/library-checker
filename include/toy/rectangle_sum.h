#pragma once
#include <toy/fenwick.h>
#include <toy/radix_sort.h>

namespace toy {
struct WeightedPoint { u32 x, y; u64 weight; };
struct Rectangle { u32 left, down, right, up; };
inline Buffer<u64> rectangle_sum(Buffer<WeightedPoint> points, std::span<const Rectangle> queries) {
    Buffer<u64> answers(queries.size()); std::fill(answers.p, answers.p + answers.n, u64(0));
    if (!points.n || queries.empty()) return answers;
    struct Coordinate { u32 value, slot; };
    Buffer<Coordinate> coordinates(points.n + 2 * queries.size());
    for (u32 i = 0; i < points.n; ++i) coordinates[i] = {points[i].y, i};
    for (u32 i = 0; i < queries.size(); ++i) {
        coordinates[points.n + 2 * i] = {queries[i].down, u32(points.n + 2 * i)};
        coordinates[points.n + 2 * i + 1] = {queries[i].up, u32(points.n + 2 * i + 1)};
    }
    radix_sort(std::span(coordinates.p, coordinates.n), [](Coordinate x) { return x.value; });
    Buffer<u32> ranks(coordinates.n); u32 count = 0;
    for (usize i = 0; i < coordinates.n; ++i) {
        if (!i || coordinates[i].value != coordinates[i - 1].value) ++count;
        ranks[coordinates[i].slot] = count - 1;
    }
    coordinates = {};
    for (usize i = 0; i < points.n; ++i) points[i].y = ranks[i];
    struct Event { u32 x, down, up, tag; }; Buffer<Event> events(2 * queries.size());
    for (u32 i = 0; i < queries.size(); ++i) {
        auto q = queries[i]; u32 down = ranks[points.n + 2 * i], up = ranks[points.n + 2 * i + 1];
        events[2 * i] = {q.left, down, up, 2 * i}; events[2 * i + 1] = {q.right, down, up, 2 * i + 1};
    }
    ranks = {}; radix_sort(std::span(points.p, points.n), [](WeightedPoint p) { return p.x; });
    radix_sort(std::span(events.p, events.n), [](Event e) { return e.x; });
    Buffer<u64> empty(count, count + 1); std::fill(empty.p, empty.p + empty.n, u64(0)); WideFenwick tree(std::move(empty));
    usize inserted = 0;
    for (auto e : std::span(events.p, events.n)) {
        while (inserted < points.n && points[inserted].x < e.x) { auto p = points[inserted++]; tree.add(p.y, p.weight); }
        u64 sum = tree.sum(e.down, e.up); answers[e.tag >> 1] += (e.tag & 1) ? sum : -sum;
    }
    return answers;
}
}
