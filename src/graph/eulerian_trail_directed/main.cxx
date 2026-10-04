#include <toy/euler_trail.h>
#include <toy/io_batch.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 tests = in.read<u32, 6>();
    while (tests--) {
        u32 n = in.read<u32, 6>(), m = in.read<u32, 6>();
        Buffer<std::array<u32, 2>> edges(m);
        read_bulk<u32, 6>(in, std::span((u32 *)edges.p, 2 * m));
        auto path = euler_trail(n, std::span<const std::array<u32, 2>>(edges), true);
        if (!path.exists) {
            out.append("No\n");
            continue;
        }
        out.append("Yes\n");
        write_bulk6(out, std::span(path.vertex.p, path.vertex.n));
        out.put('\n');
        write_bulk6(out, std::span(path.edge.p, path.edge.n));
        out.put('\n');
    }
}
