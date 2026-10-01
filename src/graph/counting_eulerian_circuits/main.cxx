#include <toy/io_batch.h>
#include <toy/spanning_count.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,3>(),m=in.read<u32,6>();Buffer<std::array<u32,2>>edges(m);read_pairs6(in,m,[&](u32 i,u32 u,u32 v){edges[i]={u,v};});out.write(euler_circuit_count(n,std::span<const std::array<u32,2>>(edges)));}
