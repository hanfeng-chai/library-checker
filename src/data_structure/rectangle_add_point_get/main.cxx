#include <toy/io.h>
#include <toy/rectangle_add_point.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), q = in.read<u32, 6>();
    RectangleAddPointGet<i64> solver(n + q, q);
    for (u32 i = 0; i < n + q; ++i) {
        u32 type = i < n ? 0 : in.read<u32, 1>();
        u32 l = in.read<u32, 10>(), d = in.read<u32, 10>();
        if (type)
            solver.query(l, d);
        else {
            u32 r = in.read<u32, 10>(), u = in.read<u32, 10>();
            i64 weight = in.read<i64, 16>();
            solver.add(l, d, r, u, weight);
        }
    }
    auto answer = solver.solve();
    out.write(std::span(answer.p, answer.n));
}
