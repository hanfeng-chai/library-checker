#include <toy/io_batch.h>
#include <toy/three_edge_components.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),m=in.read<u32,6>();Buffer<std::array<u32,2>>edges(m);read_pairs6(in,m,[&](u32 i,u32 u,u32 v){edges[i]={u,v};});auto g=indexed_graph(n,std::span<const std::array<u32,2>>(edges));edges={};auto groups=three_edge_components(g);write6(out,groups.size());for(u32 i=0;i<groups.size();++i){auto row=groups[i];write6(out,row.size(),' ');write_bulk6(out,row);out.put('\n');}}
