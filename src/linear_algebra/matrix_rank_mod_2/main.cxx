#include <toy/io.h>
#include <toy/bit_matrix.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,8>(),m=in.read<u32,8>();if(n>m&&m<=256){u32 words=(m+63)/64,rank=0;Buffer<u64>basis(m*words),row(words);std::fill(basis.p,basis.p+basis.n,0ull);for(u32 i=0;i<n&&rank<m;++i){std::fill(row.p,row.p+words,0ull);pack_bits(row.p,in.token());for(u32 w=0;w<words;++w)while(row[w]){u32 bit=64*w+std::countr_zero(row[w]);u64*p=basis.p+bit*words;if(!p[w]){memcpy(p,row.p,words*8);++rank;goto next_row;}xor_words(row.p,p,w,words);} next_row:; }out.write(rank);return 0;}BitMatrix a(n,m);for(u32 i=0;i<n;++i)pack_bits(a[i],in.token());out.write(a.eliminate(m));}
