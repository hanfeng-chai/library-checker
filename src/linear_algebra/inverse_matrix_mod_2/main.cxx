#include <toy/io.h>
#include <toy/bit_inverse.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,4>();BitMatrix a(n,2*n);for(u32 i=0;i<n;++i){pack_bits(a[i],in.token());a.set(i,n+i);}if(a.eliminate(n)<n){out.write(-1);return 0;}bit_back_substitute(a,n);Buffer<u32>where(n);for(u32 i=0;i<n;++i)where[a.pivot[i]]=i;Buffer<char>text(n);for(u32 i=0;i<n;++i){unpack_bits(a[where[i]],n,text);out.append({text.p,n});out.put('\n');}}
