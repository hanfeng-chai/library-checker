#include <toy/io_batch.h>
#include <toy/dominator.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),m=in.read<u32,6>(),s=in.read<u32,6>();Buffer<std::array<u32,2>>edges(m);read_bulk<u32,6>(in,std::span((u32*)edges.p,2*m));
 auto answer=dominator_tree(n,std::span<const std::array<u32,2>>(edges),s);for(auto x:std::span(answer.p,answer.n))out.write(x,' ');out.put('\n');}
