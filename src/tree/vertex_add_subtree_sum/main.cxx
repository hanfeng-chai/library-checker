#include <toy/preorder.h>
#include <toy/fenwick.h>
#include <toy/io.h>
using namespace toy;
int main(){Reader in;Writer out;auto[n,q]=in.read_pair<6>();Buffer<u64>ordered(n);struct Op{u32 first,second;};Buffer<Op>ops(q);u32 queries=0;
    {Buffer<u64>a(n);for(auto& x:std::span(a.p,a.n))x=in.read<u32,10>();Buffer<u32>p(n);p[0]=0;for(u32 v=1;v<n;++v)p[v]=in.read<u32,6>();OrderedPreorder tree{std::span<const u32>(p)};for(u32 v=0;v<n;++v)ordered[tree.position[v]]=a[v];if(n==1&&q)while(*in.p<=' ')++in.p;
        for(auto& op:std::span(ops.p,ops.n)){u32 type=in.read<u32,1>(),v=in.read<u32,6>();op.first=tree.position[v]|(type<<31);if(type){op.second=tree.position[v]+tree.size[v];++queries;}else op.second=in.read<u32,10>();}}
    WideFenwick sum(std::move(ordered));Buffer<u64>answers(queries);u32 at=0;
    for(auto op:std::span(ops.p,ops.n)){if(op.first>>31)answers[at++]=sum.sum(op.first&0x7fffffff,op.second);else sum.add(op.first,op.second);}out.write(std::span(answers.p,answers.n));}
