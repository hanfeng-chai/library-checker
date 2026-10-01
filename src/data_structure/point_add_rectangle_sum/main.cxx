#include <toy/io.h>
#include <toy/wavelet_sum.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>(),count=n;Buffer<u32>x(n+q),y(n+q);Buffer<u64>w(n+q);struct Op{u32 type,a,b,c,d;};Buffer<Op>ops(q);
    for(u32 i=0;i<n;++i){x[i]=in.read<u32,10>();y[i]=in.read<u32,10>();w[i]=in.read<u32,10>();}
    for(auto& op:std::span(ops.p,ops.n)){u32 type=in.read<u32,1>(),a=in.read<u32,10>(),b=in.read<u32,10>();if(!type){u32 value=in.read<u32,10>();x[count]=a;y[count]=b;w[count]=0;op={0,count++,value,0,0};}else op={1,a,b,in.read<u32,10>(),in.read<u32,10>()};}
    WaveletSum tree{std::span<const u32>(x.p,count),std::span<const u32>(y.p,count),std::span<const u64>(w.p,count)};
    for(auto op:std::span(ops.p,ops.n))if(!op.type)tree.add(op.a,u64(op.b));else out.write(tree.rectangle(op.a,op.b,op.c,op.d));}
