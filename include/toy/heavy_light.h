#pragma once
#include <toy/buffer.h>
namespace toy {
struct HeavyLight {
    Buffer<u32> parent,depth,size,heavy,head,position,vertex,degree;
    explicit HeavyLight(u32 n):parent(n),depth(n),size(n),heavy(n),head(n),position(n),vertex(n),degree(n){
        std::fill(parent.p,parent.p+n,0u);std::fill(degree.p,degree.p+n,0u);std::fill(size.p,size.p+n,1u);std::fill(heavy.p,heavy.p+n,~0u);}
    void add_edge(u32 a,u32 b){++degree[a];++degree[b];parent[a]^=b;parent[b]^=a;}
    void build(u32 root=0){u32 n=parent.n;if(!n)return;Buffer<u32> order(n);u32 at=n;++degree[root];
        // Peeling leaves leaves their parent in the xor slot; reverse removal order is topological.
        for(u32 start=0;start<n;++start){u32 v=start;while(v!=root&&degree[v]==1){u32 p=parent[v];order[--at]=v;size[p]+=size[v];if(heavy[p]==~0u||size[v]>size[heavy[p]])heavy[p]=v;
            degree[v]=0;--degree[p];parent[p]^=v;v=p;}}
        order[0]=root;parent[root]=root;depth[root]=position[root]=0;head[root]=root;degree[root]=n;
        // The heavy child's interval comes first. Allocate light subtrees backward from the end.
        for(u32 i=1;i<n;++i){u32 v=order[i],p=parent[v];depth[v]=depth[p]+1;
            if(heavy[p]==v){position[v]=position[p]+1;head[v]=head[p];}else{position[v]=degree[p]-=size[v];head[v]=v;}degree[v]=position[v]+size[v];}
        for(u32 v=0;v<n;++v)vertex[position[v]]=v;degree={};
    }
    u32 lca(u32 a,u32 b)const{while(head[a]!=head[b]){if(depth[head[a]]<depth[head[b]])std::swap(a,b);a=parent[head[a]];}return depth[a]<depth[b]?a:b;}
    u32 ancestor(u32 v,u32 distance)const{if(distance>depth[v])return ~0u;while(distance>position[v]-position[head[v]]){distance-=position[v]-position[head[v]]+1;v=parent[head[v]];}return vertex[position[v]-distance];}
    u32 jump(u32 a,u32 b,u32 distance)const{if(!distance)return a;if(distance>depth[a]+depth[b])return ~0u;u32 c=lca(a,b),up=depth[a]-depth[c],length=up+depth[b]-depth[c];
        if(distance>length)return ~0u;return distance<=up?ancestor(a,distance):ancestor(b,length-distance);}
};
}
