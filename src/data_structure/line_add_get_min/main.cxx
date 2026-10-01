#include <toy/io.h>
#include <toy/li_chao.h>
#include <toy/line_hull.h>
#include <toy/radix_sort.h>
using namespace toy;
int main(){
    Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>();Buffer<Line> ops(n+q);Buffer<u64> events(0,q);
    for(u32 i=0;i<n+q;++i){u32 type=i<n?0:in.read<u32,1>();i32 x=in.read<i32,10>();
        ops[i]={x,type?Line::infinity+1:in.read<i64,19>()};if(type)events[events.n++]=(u64(i)<<32)|(u32(x)^0x80000000u);}
    radix_sort(std::span(events.p,events.n),[](u64 x){return u32(x);});Buffer<i32> xs(0,events.n);
    for(auto event:std::span(events.p,events.n)){i32 x=i32(u32(event)^0x80000000u);if(!xs.n||xs[xs.n-1]!=x)xs[xs.n++]=x;ops[event>>32].slope=xs.n-1;}
    LiChaoTree tree{std::span<const i32>(xs)};
    if(xs.n){Buffer<Line> initial(n);memcpy(initial.p,ops.p,n*sizeof(Line));LineHull hull(std::move(initial));hull.evaluate(std::span<const i32>(xs),std::span(tree.leaves.p,xs.n));}
    for(auto op:std::span(ops.p+n,q))if(op.intercept<=Line::infinity)tree.add(op);else out.write(tree.minimum(op.slope));
}
