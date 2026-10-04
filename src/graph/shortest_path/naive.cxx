#include <toy/io_batch.h>
#include <toy/shortest_path.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>(), s = in.read<u32, 6>(), t = in.read<u32, 6>();
    Buffer<std::array<u32, 3>> edges(m);
    for (auto &e : std::span(edges.p, edges.n)) {
        e[0] = in.read<u32, 6>();
        e[1] = in.read<u32, 6>();
        e[2] = in.read<u32, 10>();
    }
    auto g = weighted_graph(n, std::span<const std::array<u32, 3>>(edges));
    edges = {};
    auto answer = shortest_path<false>(g, s, t);
    if (answer.distance == ~0ull) {
        out.write(-1);
        return 0;
    }
    out.write(answer.distance, ' ');
    write6(out, answer.vertex.n - 1);
    for (usize i = 1; i < answer.vertex.n; ++i) {
        write6(out, answer.vertex[i - 1], ' ');
        write6(out, answer.vertex[i]);
    }
}
