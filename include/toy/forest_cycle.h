#pragma once
#include <toy/dsu.h>
#include <toy/graph_cycle.h>
namespace toy {
// Union by size reroots only the smaller tree. Across all insertions, rerooting
// costs O(n log n); most joins of an isolated vertex do not flip any edge.
struct ForestCycle {
    DSU dsu;Buffer<u32> parent,edge;u32 size=0,from=0,to=0,closing=0;
    explicit ForestCycle(u32 n,u32=0):dsu(n),parent(n),edge(n){std::iota(parent.p,parent.p+n,0u);}
    bool add(u32 u,u32 v){
        u32 id=size++,a=dsu.leader(u),b=dsu.leader(v);
        if(a==b){from=u;to=v;closing=id;return true;}
        if(dsu.parent[a]<dsu.parent[b]){std::swap(a,b);std::swap(u,v);}
        u32 x=u,previous=u,previous_edge=~0u;
        for(;;){u32 next=parent[x],old_edge=next==x?~0u:edge[x];parent[x]=previous;edge[x]=previous_edge;if(next==x)break;previous=x;previous_edge=old_edge;x=next;}
        parent[u]=v;edge[u]=id;dsu.parent[b]+=dsu.parent[a];dsu.parent[a]=b;return false;
    }
    GraphCycle cycle()const{
        u32 meet=from;
        if(from!=to){
            Buffer<u8> seen(parent.n);memset(seen.p,0,seen.n);u32 u=from,v=to;seen[u]=1;seen[v]=2;
            for(;;){
                if(parent[u]!=u){u=parent[u];if(seen[u]==2){meet=u;break;}seen[u]=1;}
                if(parent[v]!=v){v=parent[v];if(seen[v]==1){meet=v;break;}seen[v]=2;}
            }
        }
        u32 left=0,right=0;for(u32 v=from;v!=meet;v=parent[v])++left;for(u32 v=to;v!=meet;v=parent[v])++right;
        u32 length=left+right+1;GraphCycle result{Buffer<u32>(length),Buffer<u32>(length)};
        u32 at=0;for(u32 v=from;v!=meet;v=parent[v]){result.vertex[at]=v;result.edge[at++]=edge[v];}result.vertex[left]=meet;
        at=length-1;for(u32 v=to;v!=meet;v=parent[v]){result.vertex[at]=v;result.edge[--at]=edge[v];}
        result.edge[length-1]=closing;return result;
    }
};
}
