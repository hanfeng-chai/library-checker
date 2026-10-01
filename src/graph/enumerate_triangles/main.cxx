#include <toy/io_batch.h>
#include <toy/small_cycles.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),m=in.read<u32,6>();Buffer<u32> weight(n);read_bulk9(in,std::span(weight.p,weight.n));
 Buffer<std::array<u32,2>> edges(m);read_bulk<u32,5>(in,std::span((u32*)edges.p,2*m));out.write(triangle_sum(std::span<const u32>(weight),std::move(edges)));
}
