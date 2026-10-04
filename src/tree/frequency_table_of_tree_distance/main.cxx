#include <toy/io.h>
#include <toy/tree_distance_histogram.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>();
    Buffer<std::array<u32, 2>> edges(n - 1);
    for (auto &e : std::span(edges.p, edges.n)) {
        auto [u, v] = in.read_pair<6>();
        e = {u, v};
    }
    Adjacency graph(n, std::span<const std::array<u32, 2>>(edges));
    auto answer = tree_distance_histogram(graph);
    out.write(std::span(answer.p + 1, answer.n - 1), ' ');
    out.put('\n');
}
