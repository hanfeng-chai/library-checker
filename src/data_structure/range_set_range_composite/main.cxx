#include <toy/io.h>
#include <toy/range_set_composite.h>
using namespace toy;
int main() {
    Reader in; Writer out; usize n = in.read<u32,6>(), q = in.read<u32,6>(); Buffer<Affine<>> a(n);
    for(auto& f:span(a.p,a.n))f={in.read<u32,16>(),in.read<u32,16>()};
    RangeSetComposite tree{span<const Affine<>>(a),q};
    while(q--) {
        u32 type=in.read<u32,1>();auto[l,r]=in.read_pair<6>();
        if(!type){u32 a=in.read<u32,16>(),b=in.read<u32,16>();tree.set(l,r,{a,b});}
        else out.write(tree.evaluate(l,r,in.read<u32,16>()));
    }
}
