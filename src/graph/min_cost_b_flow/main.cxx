#include <toy/io_batch.h>
#include <toy/network_simplex.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,3>(),m=in.read<u32,4>();NetworkSimplex graph(n,m);for(u32 v=0;v<n;++v)graph.balance[v]=in.read<i64,10>();for(u32 i=0;i<m;++i){u32 u=in.read<u32,2>(),v=in.read<u32,2>();i64 lo=in.read<i64,10>(),hi=in.read<i64,10>(),cost=in.read<i64,10>();graph.add(u,v,lo,hi,cost);}if(!graph.solve()){out.append("infeasible\n");return 0;}out.write(graph.cost);out.write(std::span(graph.potential.p,n));out.write(std::span(graph.flow.p,graph.flow.n));}
