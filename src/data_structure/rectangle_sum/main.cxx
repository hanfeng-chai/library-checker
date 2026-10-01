#include <toy/io.h>
#include <toy/rectangle_sum.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32, 6>(), q = in.read<u32, 6>(); Buffer<WeightedPoint> points(n);
    for (auto& p : std::span(points.p, points.n)) p = {in.read<u32, 10>(), in.read<u32, 10>(), in.read<u64, 10>()};
    Buffer<Rectangle> queries(q);
    for (auto& r : std::span(queries.p, queries.n)) r = {in.read<u32, 10>(), in.read<u32, 10>(), in.read<u32, 10>(), in.read<u32, 10>()};
    auto answers = rectangle_sum(std::move(points), std::span<const Rectangle>(queries)); out.write(std::span(answers.p, answers.n));
}
