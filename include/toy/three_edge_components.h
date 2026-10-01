#pragma once
#include <toy/graph.h>
#include <toy/dsu.h>
#include <toy/vertex_groups.h>
namespace toy {
// DFS path contraction for 3-edge connectivity in undirected multigraphs.
// degree counts unabsorbed back edges; path links the still exposed chain.
inline VertexGroups three_edge_components(const Graph<IndexedArc>&g){
    u32 n=g.size();Buffer<i32>pre(n),post(n),path(n),low(n),degree(n);std::fill(pre.p,pre.p+n,-1);std::fill(path.p,path.p+n,-1);std::fill(degree.p,degree.p+n,0);DSU dsu(n);i32 time=0;
    struct Frame{u32 v,next,parent_edge;};Buffer<Frame>stack(n);u32 depth=0;
    auto enter=[&](u32 v,u32 edge){pre[v]=low[v]=time++;stack[depth++]={v,g.offset[v],edge};};
    auto absorb=[&](u32 u,i32 v){if(path[v]<0&&degree[v]<=1){low[u]=std::min(low[u],low[v]);degree[u]+=degree[v];return;}if(!degree[v])v=path[v];if(low[u]>low[v]){low[u]=low[v];std::swap(v,path[u]);}while(v>=0){dsu.merge(u,v);degree[u]+=degree[v];v=path[v];}};
    for(u32 root=0;root<n;++root)if(pre[root]<0){enter(root,~0u);
        while(depth){auto& f=stack[depth-1];u32 u=f.v;if(f.next==g.offset[u+1]){post[u]=time-1;--depth;if(depth)absorb(stack[depth-1].v,u);continue;}
            auto e=g.edge[f.next++];u32 v=e.to;if(u==v||e.id==f.parent_edge)continue;if(pre[v]<0){enter(v,e.id);continue;}
            if(pre[v]<pre[u]){++degree[u];low[u]=std::min(low[u],pre[v]);}
            else{--degree[u];for(i32& p=path[u];p>=0&&pre[p]<=pre[v]&&pre[v]<=post[p];p=path[p]){dsu.merge(u,p);degree[u]+=degree[p];}}
        }
    }
    Buffer<u32>head(n),next(n);std::fill(head.p,head.p+n,~0u);for(u32 v=0;v<n;++v){u32 r=dsu.leader(v);next[v]=head[r];head[r]=v;}VertexGroups result(n,n);
    for(u32 r=0;r<n;++r)if(head[r]!=~0u){for(u32 v=head[r];v!=~0u;v=next[v])result.add(v);result.finish();}return result;
}
}
