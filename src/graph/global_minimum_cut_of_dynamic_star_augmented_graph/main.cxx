#include <toy/io_batch.h>
#include <toy/apex_min_cut.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,4>(),m=in.read<u32,4>(),q=in.read<u32,6>();Buffer<i64>weight(n);for(auto&x:std::span(weight.p,weight.n))x=in.read<u32,10>();Buffer<std::array<u32,3>>edges(m);read_triples(in,m,[&](u32 i,u32 u,u32 v,u32 w){edges[i]={u,v,w};});ApexMinCut cut(weight,edges);while(q--){u32 v=in.read<u32,4>();i64 value=in.read<u32,10>();out.write(cut.set(v,value));}}
