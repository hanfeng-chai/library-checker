#include <toy/io.h>
#include <toy/li_chao.h>
#include <toy/radix_sort.h>
using namespace toy;
int main(){
    Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>();struct Op{u32 l,r;Line line;};Buffer<Op> ops(n+q);Buffer<u64> events(0,2*(n+q));
    auto event=[&](u32 i,u32 kind,i32 x){events[events.n++]=(u64(4*i+kind)<<32)|(u32(x)^0x80000000u);};
    for(u32 i=0;i<n+q;++i){u32 type=i<n?0:in.read<u32,1>();
        if(!type){i32 l=in.read<i32,10>(),r=in.read<i32,10>(),a=in.read<i32,10>();i64 b=in.read<i64,19>();ops[i]={0,0,{a,b}};event(i,0,l);event(i,1,r);}
        else{ops[i]={0,0,{0,Line::infinity+1}};event(i,2,in.read<i32,10>());}}
    radix_sort(std::span(events.p,events.n),[](u64 x){return u32(x);});Buffer<i32> xs(0,q);
    for(auto event:std::span(events.p,events.n)){i32 x=i32(u32(event)^0x80000000u);u32 tag=event>>32;auto& op=ops[tag/4];bool same=xs.n&&xs[xs.n-1]==x;
        if((tag&3)==2){if(!same)xs[xs.n++]=x;op.l=xs.n-1;}else{u32 at=xs.n-same;if(tag&1)op.r=at;else op.l=at;}}
    LiChaoTree tree{std::span<const i32>(xs)};
    for(auto op:std::span(ops.p,ops.n)){if(op.line.intercept<=Line::infinity)tree.add_segment(op.l,op.r,op.line);else{i64 answer=tree.minimum(op.l);if(answer==Line::infinity)out.append("INFINITY\n");else out.write(answer);}}
}
