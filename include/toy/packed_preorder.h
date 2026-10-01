#pragma once
#include <toy/buffer.h>
namespace toy {
struct TreePreorder {
    struct Node{u32 parent=0,degree=0,position=1,depth=0;};
    Buffer<Node> nodes;Buffer<u32>position,vertex,parent,depth;
    explicit TreePreorder(u32 n):nodes(n),position(n),vertex(n),parent(n),depth(n){std::fill(nodes.p,nodes.p+n,Node{});}
    void add_edge(u32 u,u32 v){++nodes[u].degree;++nodes[v].degree;nodes[u].parent^=v;nodes[v].parent^=u;}
    void build(u32 root=0){u32 n=nodes.n;if(!n)return;Buffer<u32>order(n);u32 at=n;++nodes[root].degree;
        for(u32 start=0;start<n;++start){u32 v=start;while(v!=root&&nodes[v].degree==1){auto& x=nodes[v];u32 p=x.parent;auto& y=nodes[p];order[--at]=v;y.position+=x.position;x.degree=0;--y.degree;y.parent^=v;v=p;}}
        order[0]=root;nodes[root].parent=root;
        for(u32 i=1;i<n;++i){auto& x=nodes[order[i]];auto& p=nodes[x.parent];u32 size=x.position;x.position=p.position;p.position-=size;x.depth=p.depth+1;}
        for(u32 v=0;v<n;++v){position[v]=--nodes[v].position;vertex[position[v]]=v;depth[position[v]]=nodes[v].depth;}
        for(u32 v=0;v<n;++v)parent[position[v]]=position[nodes[v].parent];nodes={};
    }
};
}
