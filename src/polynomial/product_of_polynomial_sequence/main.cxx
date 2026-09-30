#include <toy/io.h>
#include <toy/product_pool.h>
using namespace toy;
int main(){
 Reader in;Writer out;usize count=in.read<u32,6>(),used=0,position=0;
 Buffer<u32>data(count+500000);Buffer<span<const u32>>f(count);u32 constant=1;
 for(usize i=0;i<count;++i){
  usize d=in.read<u32,6>();if(!d){constant=Mod<998244353>::mul(constant,in.read<u32,9>());continue;}
  f[used++]={data.p+position,d+1};for(usize j=0;j<=d;++j)data[position++]=in.read<u32,9>();
 }
 auto g=polynomial_product_pool(span<const span<const u32>>(f.p,used));fps_scale(g,constant);out.write(span(g.p,g.n),' ');
}
