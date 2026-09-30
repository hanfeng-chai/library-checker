#include <toy/io.h>
#include <toy/weighted_affine.h>
#include <toy/radix_sort.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,10>(),q=in.read<u32,6>();struct Op{u32 type,l,r,a,b;};Buffer<Op> ops(q);Buffer<u64> events(2*q+2);events[2*q]=u64(2*q)<<32;events[2*q+1]=(u64(2*q+1)<<32)|n;
    for(u32 i=0;i<q;++i){auto& op=ops[i];op={in.read<u32,1>(),in.read<u32,10>(),in.read<u32,10>(),0,0};if(!op.type){op.a=in.read<u32,16>();op.b=in.read<u32,16>();}events[2*i]=(u64(2*i)<<32)|op.l;events[2*i+1]=(u64(2*i+1)<<32)|op.r;}
    radix_sort(span(events.p,events.n),[](u64 x){return u32(x);});Buffer<u32> coordinates(0,events.n);
    for(u64 e:span(events.p,events.n)){u32 x=e,tag=e>>32;if(!coordinates.n||coordinates[coordinates.n-1]!=x)coordinates[coordinates.n++]=x;if(tag<2*q){if(tag&1)ops[tag/2].r=coordinates.n-1;else ops[tag/2].l=coordinates.n-1;}}
    WeightedAffineSum tree{span<const u32>(coordinates)};for(auto op:span(ops.p,ops.n))if(op.type)out.write(tree.sum(op.l,op.r));else tree.apply(op.l,op.r,{op.a,op.b});}
