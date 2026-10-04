#include <toy/assignment.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 3>();
    Buffer<i32> cost(n * n);
    for (auto &x : std::span(cost.p, cost.n)) x = in.read<i32, 10>();
    auto answer = assignment(n, std::span<const i32>(cost));
    out.write(answer.cost);
    write_bulk6(out, std::span(answer.column.p, answer.column.n));
    out.put('\n');
}
