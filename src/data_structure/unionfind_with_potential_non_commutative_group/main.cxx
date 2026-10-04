#include <toy/group.h>
#include <toy/io.h>
#include <toy/potential_dsu.h>
using namespace toy;
int main() {
    Reader in;
    Writer out;
    usize n = in.read<u32, 6>(), q = in.read<u32, 6>();
    using Group = Matrix2Group<>;
    PotentialDSU<Group> dsu(n);
    while (q--) {
        u32 type = in.read<u32, 1>();
        auto [a, b] = in.read_pair<6>();
        if (!type) {
            Group::Value x{in.read<u32, 9>(), in.read<u32, 9>(), in.read<u32, 9>(),
                           in.read<u32, 9>()};
            out.put('0' + dsu.merge(a, b, x));
            out.put('\n');
        } else {
            auto x = dsu.difference(a, b);
            if (!x)
                out.write(-1);
            else {
                out.write(x->a, ' ');
                out.write(x->b, ' ');
                out.write(x->c, ' ');
                out.write(x->d);
            }
        }
    }
}
