#pragma once
#include <toy/buffer.h>
namespace toy {
struct Adjacency {
    Buffer<u32> offset,edge;
    Adjacency(u32 n,std::span<const std::array<u32,2>> input,bool directed=false):offset(n+1),edge(input.size()*(directed?1:2)){
        std::fill(offset.p,offset.p+offset.n,0u);for(auto[u,v]:input){++offset[u+1];if(!directed)++offset[v+1];}for(u32 i=1;i<=n;++i)offset[i]+=offset[i-1];Buffer<u32>next(n);if(n)memcpy(next.p,offset.p,4*n);
        for(auto[u,v]:input){edge[next[u]++]=v;if(!directed)edge[next[v]++]=u;}
    }
    u32 size()const{return offset.n-1;}
    std::span<const u32> operator[](u32 v)const{return {edge.p+offset[v],offset[v+1]-offset[v]};}
};
}
