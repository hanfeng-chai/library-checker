#include <toy/io_batch.h>
#include <toy/biconnected.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),m=in.read<u32,6>();Buffer<std::array<u32,2>> edges(m);
 read_bulk<u32,6>(in,std::span((u32*)edges.p,2*m));auto g=indexed_graph(n,std::span<const std::array<u32,2>>(edges));BiconnectedComponents groups(g);
 write6(out,groups.size());for(u32 i=0;i<groups.size();++i){auto g=groups[i];write6(out,g.size(),' ');write_bulk6(out,g);out.put('\n');}
}
