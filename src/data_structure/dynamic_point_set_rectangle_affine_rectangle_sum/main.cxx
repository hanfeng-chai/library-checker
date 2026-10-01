#include <toy/io.h>
#include <toy/kd_affine.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>(),count=n;Buffer<u32>x(n+q),y(n+q),values(n+q);struct Op{u32 type,a,b,c,d,e,f;};Buffer<Op> ops(q);
    for(u32 i=0;i<n;++i){x[i]=in.read<u32,16>();y[i]=in.read<u32,16>();values[i]=in.read<u32,16>();}
    for(auto& op:std::span(ops.p,ops.n)){op={};op.type=in.read<u32,1>();if(!op.type){x[count]=in.read<u32,16>();y[count]=in.read<u32,16>();values[count]=0;op.a=count++;op.b=in.read<u32,16>();}
        else if(op.type==1){op.a=in.read<u32,6>();op.b=in.read<u32,16>();}
        else{op.a=in.read<u32,16>();op.b=in.read<u32,16>();op.c=in.read<u32,16>();op.d=in.read<u32,16>();if(op.type==3){op.e=in.read<u32,16>();op.f=in.read<u32,16>();}}}
    AffineKDTree tree{std::span<const u32>(x.p,count),std::span<const u32>(y.p,count),std::span<const u32>(values.p,count),n};
    for(auto op:std::span(ops.p,ops.n))if(op.type<2)tree.set(op.a,op.b);else if(op.type==2)out.write(tree.sum(op.a,op.b,op.c,op.d));else tree.apply(op.a,op.b,op.c,op.d,{op.e,op.f});}
