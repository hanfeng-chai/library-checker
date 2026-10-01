#include <toy/io_batch.h>
#include <toy/clique.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,2>(),m=in.read<u32,3>();Buffer<u64>g(n);u64 all=(1ull<<n)-1;std::fill(g.p,g.p+n,0ull);
 for(u32 i=0;i<m;++i){u32 u=in.read<u32,2>(),v=in.read<u32,2>();g[u]|=1ull<<v;g[v]|=1ull<<u;}
 u64 answer=independent_set_mitm(std::span<const u64>(g));out.write(std::popcount(answer));while(answer){u32 v=std::countr_zero(answer);answer&=answer-1;out.write(v,' ');}out.put('\n');}
