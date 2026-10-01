#include <toy/io_batch.h>
#include <toy/packed_digraph.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),m=in.read<u32,6>();PackedDigraph g(n,m);
 std::array<std::array<u32,2>,16> pending;
 read_pairs6(in,m,[&](u32 i,u32 u,u32 v){__builtin_prefetch(g.vertex.p+u,1);if(i>=16){auto e=pending[i%16];g.add(e[0],e[1]);}pending[i%16]={u,v};});
 for(u32 i=m>16?m-16:0;i<m;++i){auto e=pending[i%16];g.add(e[0],e[1]);}
 auto groups=g.components();write6(out,groups.size());for(u32 i=0;i<groups.size();++i){auto row=groups[i];write6(out,row.size(),' ');write_bulk6(out,row);out.put('\n');}
}
