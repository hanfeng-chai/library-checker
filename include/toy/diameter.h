#pragma once
#include <toy/buffer.h>
namespace toy {
// A single-use leaf-peeling diameter solver for nonnegative u32 edge weights.
struct TreeDiameter {
    struct Result{u64 length;Buffer<u32> path;};
    Buffer<u32> degree,neighbor,weight,parent,endpoint;Buffer<u64> down;
    explicit TreeDiameter(u32 n):degree(n),neighbor(n),weight(n),parent(n),endpoint(n),down(n){
        std::fill(degree.p,degree.p+n,0u);std::fill(neighbor.p,neighbor.p+n,0u);std::fill(weight.p,weight.p+n,0u);std::fill(down.p,down.p+n,0ull);std::iota(endpoint.p,endpoint.p+n,0u);}
    void add_edge(u32 a,u32 b,u32 w){++degree[a];++degree[b];neighbor[a]^=b;neighbor[b]^=a;weight[a]^=w;weight[b]^=w;}
    Result solve(u32 root=0){u32 n=degree.n;if(!n)return {0,{}};++degree[root];u64 best=0;u32 a=root,b=root,center=root;
        for(u32 start=0;start<n;++start){u32 v=start;while(degree[v]==1){if(v==root){degree[v]=0;break;}u32 p=neighbor[v],w=weight[v];u64 next=down[v]+w;parent[v]=p;
            if(next+down[p]>best){best=next+down[p];a=endpoint[v];b=endpoint[p];center=p;}if(next>down[p]){down[p]=next;endpoint[p]=endpoint[v];}
            degree[v]=0;--degree[p];neighbor[p]^=v;weight[p]^=w;v=p;}}
        Buffer<u32> path(n),tail(n);path.n=tail.n=0;while(a!=center){path[path.n++]=a;a=parent[a];}path[path.n++]=center;while(b!=center){tail[tail.n++]=b;b=parent[b];}
        while(tail.n)path[path.n++]=tail[--tail.n];return {best,std::move(path)};
    }
};
}
