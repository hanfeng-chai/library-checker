#pragma once
#include <toy/determinant.h>
#include <toy/dsu.h>
namespace toy {
// In-degree Laplacian minor: det counts arborescences pointing away from root.
template<u32 P=998244353>struct SpanningCount {
    u32 n,root;Buffer<u32>matrix;
    SpanningCount(u32 vertices,u32 root):n(vertices-1),root(root),matrix(usize(n)*n){std::fill(matrix.p,matrix.p+matrix.n,0u);}
    void add(u32 u,u32 v){if(u==v||v==root)return;v-=v>root;++matrix[usize(v)*n+v];if(u!=root){u-=u>root;--matrix[usize(v)*n+u];}}
    template<bool Wide=true>u32 count(){for(auto&x:std::span(matrix.p,matrix.n))if(i32(x)<0)x+=P;return determinant<P,Wide>(n,matrix);}
};
template<u32 P=998244353,bool Wide=true>u32 euler_circuit_count(u32 n,std::span<const std::array<u32,2>>edges){
    if(edges.empty())return 0;Buffer<u32>in(n),out(n),index(n);std::fill(in.p,in.p+n,0u);std::fill(out.p,out.p+n,0u);DSU dsu(n);
    for(auto e:edges){++out[e[0]];++in[e[1]];dsu.merge(e[0],e[1]);}u32 active=0,root=edges[0][0];
    for(u32 v=0;v<n;++v){if(in[v]!=out[v])return 0;if(out[v]){if(!dsu.same(root,v))return 0;index[v]=active++;}}
    SpanningCount<P>laplace(active,active-1);for(auto e:edges)laplace.add(index[e[1]],index[e[0]]);u32 answer=laplace.template count<Wide>();
    Buffer<u32>factorial(edges.size()+1);factorial[0]=1;for(u32 i=1;i<factorial.n;++i)factorial[i]=Mod<P>::mul(factorial[i-1],i);
    for(u32 v=0;v<n;++v)if(out[v])answer=Mod<P>::mul(answer,factorial[out[v]-1]);return answer;
}
}
