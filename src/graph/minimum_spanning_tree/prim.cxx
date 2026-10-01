#include <toy/io_batch.h>
#include <toy/spanning_tree.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),m=in.read<u32,6>();Buffer<std::array<u32,3>> edges(m);
 for(auto& e:std::span(edges.p,edges.n)){e[0]=in.read<u32,6>();e[1]=in.read<u32,6>();e[2]=in.read<u32,10>();}
 auto tree=prim(n,std::span<const std::array<u32,3>>(edges));out.write(tree.cost);write_bulk6(out,std::span(tree.edge.p,tree.edge.n));out.put('\n');
}
