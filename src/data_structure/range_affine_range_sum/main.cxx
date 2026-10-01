#include <toy/io.h>
#include <toy/affine_range_sum.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>();Buffer<u32>a(n);for(u32 i=0;i+1<n;i+=2){auto[x,y]=in.read_pair<9>();a[i]=x;a[i+1]=y;}if(n&1)a[n-1]=in.read<u32,16>();
    struct Op{u32 l,r,a,b;};Buffer<Op> ops(q);
    for(auto& op:std::span(ops.p,ops.n)){u32 type=in.read<u32,1>();auto[l,r]=in.read_pair<6>();op.l=l|(type<<31);op.r=r;if(!type){auto[x,y]=in.read_pair<9>();op.a=x;op.b=y;}else op.a=op.b=0;}
    AffineRangeSum tree{std::span<const u32>(a)};
    for(u32 i=0;i<q;++i){if(i+4<q)tree.prefetch(ops[i+4].l&0x7fffffff,ops[i+4].r);auto op=ops[i];if(op.l>>31)out.write(tree.sum(op.l&0x7fffffff,op.r));else tree.apply(op.l,op.r,{op.a,op.b});}}
