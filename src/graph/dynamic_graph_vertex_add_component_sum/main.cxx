#include <toy/io_batch.h>
#include <toy/expiry_forest.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>();Buffer<u64>initial(n);read_bulk<u64,10>(in,std::span(initial.p,initial.n));Buffer<GraphOperation>operations(q);for(auto&o:std::span(operations.p,operations.n)){o.type=in.read<u32,1>();o.u=in.read<u32,6>();o.v=o.type==3?0:o.type<2?in.read<u32,6>():in.read<u32,10>();}auto answer=expiry_component_sum(initial,operations);out.write(std::span(answer.p,answer.n));}
