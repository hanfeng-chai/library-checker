#include <toy/io_batch.h>
#include <toy/shortest_path.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>(), s = in.read<u32, 6>(), t = in.read<u32, 6>();
    Buffer<std::array<u32, 3>> edges(m);
    read_triple_rows(in, std::span(edges.p, edges.n));
    auto special = single_predecessor_path(n, edges, s, t);
    ShortestPath answer;
    if (special)
        answer = std::move(*special);
    else {
        u32 outgoing = 0, incoming = 0;
        for (auto e : std::span(edges.p, edges.n)) {
            outgoing += e[0] == s;
            incoming += e[1] == t;
        }
        bool reverse = outgoing > incoming;
        if (reverse) {
            for (auto &e : std::span(edges.p, edges.n)) std::swap(e[0], e[1]);
            std::swap(s, t);
        }
        auto g = weighted_graph(n, std::span<const std::array<u32, 3>>(edges));
        edges = {};
        answer = shortest_path<true, true>(g, s, t);
        if (reverse) std::reverse(answer.vertex.p, answer.vertex.p + answer.vertex.n);
    }
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
