#include <toy/io.h>
#include <toy/bit_matrix.h>
using namespace toy;
int main(){Reader in;Writer out;u32 t=in.read<u32,6>();while(t--){u32 n=in.read<u32,2>();std::array<u32,30>u,v;for(u32 i=0;i<n;++i)u[i]=in.read<u32,10>();u32 m=in.read<u32,2>();for(u32 i=0;i<m;++i)v[i]=in.read<u32,10>();auto a=binary_intersection<30>(std::span(u.data(),n),std::span(v.data(),m));out.write(a.n,' ');out.write(std::span(a.p,a.n),' ');out.put('\n');}}
