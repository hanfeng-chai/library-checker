#include <toy/io_batch.h>
#include <toy/am_tree.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),m=in.read<u32,7>();AMForest forest(n);std::array<std::array<u32,3>,16>pending;
 auto apply=[&](u32 id){auto e=pending[id%16];out.write(forest.add(e[0],e[1],e[2],id),' ');};
 read_triples(in,m,[&](u32 id,u32 u,u32 v,u32 w){__builtin_prefetch(forest.node.p+u+1,1);__builtin_prefetch(forest.node.p+v+1,1);if(id>=16)apply(id-16);pending[id%16]={u,v,w};});
 for(u32 id=m>16?m-16:0;id<m;++id)apply(id);out.put('\n');}
