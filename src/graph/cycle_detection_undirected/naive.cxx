#include <toy/io_batch.h>
#include <toy/graph_cycle.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),m=in.read<u32,6>();
 Buffer<std::array<u32,2>> edges(m);read_bulk<u32,6>(in,std::span((u32*)edges.p,2*m));
 auto g=indexed_graph(n,std::span<const std::array<u32,2>>(edges),false);auto c=graph_cycle(g,false);
 if(!c.edge.n){out.write(-1);return 0;}write6(out,c.edge.n);
 write_bulk6(out,std::span(c.vertex.p,c.vertex.n));out.put('\n');
 write_bulk6(out,std::span(c.edge.p,c.edge.n));out.put('\n');
}
