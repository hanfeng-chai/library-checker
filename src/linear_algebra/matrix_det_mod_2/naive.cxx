#include <toy/io.h>
#include <toy/bit_gaussian.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,4>();BitMatrix a(n,n);for(u32 i=0;i<n;++i)pack_bits(a[i],in.token());out.write(u32(bit_gaussian(a,n)==n));}
