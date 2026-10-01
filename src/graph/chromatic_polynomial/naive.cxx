#include <toy/io_batch.h>
#include <toy/chromatic.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,2>(),m=in.read<u32,3>();Buffer<u32>g(n);std::fill(g.p,g.p+n,0u);for(u32 i=0;i<m;++i){u32 u=in.read<u32,2>(),v=in.read<u32,2>();g[u]|=1u<<v;g[v]|=1u<<u;}auto answer=chromatic_polynomial<998244353,false>(g);out.write(std::span(answer.p,answer.n),' ');out.put('\n');}
