#pragma once
#include <toy/graph.h>
namespace toy {
struct EulerTrail {bool exists;Buffer<u32> vertex,edge;};
inline EulerTrail euler_trail(u32 n,std::span<const std::array<u32,2>> edges,bool directed){
    u32 m=edges.size(),start=0;
    if(m<2){EulerTrail r{true,Buffer<u32>(m+1),Buffer<u32>(m)};r.vertex[0]=m?edges[0][0]:0;if(m){r.vertex[1]=edges[0][1];r.edge[0]=0;}return r;}
    Buffer<i32> degree(n);std::fill(degree.p,degree.p+n,0);
    for(auto e:edges){++degree[e[0]];if(directed)--degree[e[1]];else ++degree[e[1]];start=e[0];}
    u32 odd=0,positive=0,negative=0;
    for(u32 v=0;v<n;++v){
        if(directed){if(degree[v]>1||degree[v]<-1)return {};if(degree[v]==1)++positive,start=v;if(degree[v]==-1)++negative;}
        else if(degree[v]&1)++odd,start=v;
    }
    if(directed?(positive!=negative||positive>1):(odd!=0&&odd!=2))return {};
    auto g=indexed_graph(n,edges,directed);Buffer<u32> cursor(n);if(n)memcpy(cursor.p,g.offset.p,n*4);
    Buffer<u8> used(directed?0:m);if(!directed&&m)memset(used.p,0,m);
    Buffer<u32> stack_v(m+1),stack_e(m+1);EulerTrail result{true,Buffer<u32>(m+1),Buffer<u32>(m)};
    u32 depth=1,count=0;stack_v[0]=start;stack_e[0]=~0u;
    while(depth){u32 v=stack_v[depth-1];auto& p=cursor[v];
        if(!directed)while(p<g.offset[v+1]&&used[g.edge[p].id])++p;
        if(p<g.offset[v+1]){auto e=g.edge[p++];if(!directed)used[e.id]=1;stack_v[depth]=e.to;stack_e[depth++]=e.id;}
        else{--depth;result.vertex[m-count]=v;if(stack_e[depth]!=~0u)result.edge[m-count-1]=stack_e[depth];++count;}
    }
    // Degrees alone do not exclude several components containing edges.
    if(count!=m+1)return {};return result;
}
}
