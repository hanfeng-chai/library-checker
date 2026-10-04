#include <toy/io.h>
#include <toy/linear_system.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    u32 n = in.read<u32, 6>(), m = in.read<u32, 6>();
    Buffer<u32> a(n * m);
    for (u32 &x : std::span(a.p, a.n)) x = in.read<u32, 9>();
    FieldEchelon e(n, m, m, std::span(a.p, a.n));
    out.write(e.pivot.n);
}
