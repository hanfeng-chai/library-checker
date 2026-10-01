#pragma once
#include <toy/adjacency.h>
#include <toy/mod.h>
namespace toy {
// Orient edges by (degree,vertex): every forward degree is O(sqrt(m)).
template<u32 P=998244353>u32 triangle_sum(std::span<const u32> weight,Buffer<std::array<u32,2>> edges){
    u32 n=weight.size();Buffer<u32>degree(n);std::fill(degree.p,degree.p+n,0u);
    for(auto e:std::span(edges.p,edges.n)){++degree[e[0]];++degree[e[1]];}
    for(auto& e:std::span(edges.p,edges.n))if(degree[e[0]]>degree[e[1]]||(degree[e[0]]==degree[e[1]]&&e[0]>e[1]))std::swap(e[0],e[1]);
    Adjacency g(n,edges,true);edges={};std::fill(degree.p,degree.p+n,0u);auto& marked=degree;u32 answer=0;
    for(u32 u=0;u<n;++u){auto next=g[u];if(!weight[u]||next.size()<2)continue;for(u32 v:next)marked[v]=weight[v];u64 total=0;
        for(u32 v:next){auto row=g[v];u64 a=0,b=0,c=0,d=0;usize i=0;
            for(;i+4<=row.size();i+=4){a+=marked[row[i]];b+=marked[row[i+1]];c+=marked[row[i+2]];d+=marked[row[i+3]];}
            a+=b+c+d;for(;i<row.size();++i)a+=marked[row[i]];
            total+=Mod<P>::mul(weight[v],a%P);
        }
        answer=Mod<P>::add(answer,Mod<P>::mul(weight[u],total%P));for(u32 v:next)marked[v]=0;
    }
    return answer;
}
}
