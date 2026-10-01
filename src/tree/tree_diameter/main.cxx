#include <toy/diameter.h>
#include <toy/io.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>();TreeDiameter tree(n);for(u32 i=1;i<n;++i){auto[a,b]=in.read_pair<6>();tree.add_edge(a,b,in.read<u32,10>());}auto result=tree.solve();out.write(result.length,' ');out.write(u32(result.path.n));out.write(std::span(result.path.p,result.path.n));}
