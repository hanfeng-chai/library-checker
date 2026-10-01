#include <toy/io_batch.h>
#include <toy/convolution_fft_split.h>
using namespace toy;
int main(){
 Reader in;Writer out;usize n=in.read<u32,8>(),m=in.read<u32,8>();
 Buffer<u32>a(n),b(m);read_bulk<u32,10>(in,std::span(a.p,a.n));read_bulk<u32,10>(in,std::span(b.p,b.n));
 auto c=convolution_fft_split<1000000007,false>(std::span<const u32>(a),std::span<const u32>(b));
 out.write(std::span(c.p,c.n),' ');
}
