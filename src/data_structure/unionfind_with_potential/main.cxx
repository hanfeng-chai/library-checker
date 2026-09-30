#include <toy/io.h>
#include <toy/group.h>
#include <toy/potential_dsu.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32, 6>(), q = in.read<u32, 6>();
    PotentialDSU<AdditiveGroup<>> dsu(n);
    while (q--) {
        u32 type = in.read<u32, 1>(); auto [a, b] = in.read_pair<6>();
        if (!type) { bool ok = dsu.merge(a, b, in.read<u32, 9>()); out.put('0' + ok); out.put('\n'); }
        else { auto x = dsu.difference(a, b); out.write(x ? i32(*x) : -1); }
    }
}
