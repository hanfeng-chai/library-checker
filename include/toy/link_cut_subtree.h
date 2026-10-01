#pragma once
#include <toy/adjacency.h>
namespace toy {
template<class T=i64> struct LinkCutSubtree {
    struct Node{T sum{},local{},add{};u32 count=1,virtual_count=0,child[2]{},parent=0;i8 side=-1;bool reversed=false;};
    Buffer<Node>nodes;
    explicit LinkCutSubtree(std::span<const T>values):nodes(values.size()+1){std::fill(nodes.p,nodes.p+nodes.n,Node{});nodes[0].count=0;for(u32 i=0;i<values.size();++i)nodes[i+1].sum=nodes[i+1].local=values[i];}
    void build_tree(const Adjacency& graph,u32 root=0){u32 n=graph.size();if(!n)return;Buffer<u32>order(n);order[0]=root+1;for(u32 i=0,count=1;i<count;++i){u32 v=order[i];for(u32 w:graph[v-1])if(w+1!=nodes[v].parent){nodes[w+1].parent=v;order[count++]=w+1;}}
        for(u32 i=n;i-->1;){auto& x=nodes[order[i]];auto& p=nodes[x.parent];p.local+=x.sum;p.virtual_count+=x.count;p.sum+=x.sum;p.count+=x.count;}}
    void pull(u32 i){auto& x=nodes[i];x.count=1+x.virtual_count+nodes[x.child[0]].count+nodes[x.child[1]].count;x.sum=x.local+nodes[x.child[0]].sum+nodes[x.child[1]].sum+x.add*x.count;}
    void apply(u32 x,T delta){if(x){nodes[x].add+=delta;nodes[x].sum+=delta*nodes[x].count;}}
    void reverse(u32 x){nodes[x].reversed^=1;}
    void push(u32 x){auto& v=nodes[x];if(v.reversed){std::swap(v.child[0],v.child[1]);nodes[v.child[0]].side=0;nodes[v.child[1]].side=1;reverse(v.child[0]);reverse(v.child[1]);v.reversed=false;}}
    void rotate(u32 x){u32 y=nodes[x].parent,z=nodes[y].parent,s=nodes[x].side,m=nodes[x].child[s^1];i8 t=nodes[y].side;T delta=nodes[x].add;
        // Differential potentials: rotations change ancestry, so compensate x, y and the middle subtree.
        nodes[x].add+=nodes[y].add;nodes[y].add=-delta;apply(m,delta);
        nodes[m].parent=y;nodes[m].side=s;nodes[y].child[s]=m;nodes[y].parent=x;nodes[y].side=s^1;nodes[x].child[s^1]=y;nodes[x].parent=z;nodes[x].side=t;if(t>=0)nodes[z].child[t]=x;pull(y);}
    void splay(u32 x){push(x);while(nodes[x].side>=0){u32 y=nodes[x].parent,z=nodes[y].parent;if(nodes[y].side>=0){push(z);push(y);push(x);rotate(nodes[x].side==nodes[y].side?y:x);}else{push(y);push(x);}rotate(x);}}
    void access(u32 x){splay(x);u32 old=nodes[x].child[1];nodes[old].side=-1;nodes[x].local+=nodes[old].sum;nodes[x].virtual_count+=nodes[old].count;nodes[x].child[1]=0;pull(x);
        while(u32 p=nodes[x].parent){splay(p);old=nodes[p].child[1];nodes[old].side=-1;nodes[p].local+=nodes[old].sum-nodes[x].sum;nodes[p].virtual_count+=nodes[old].count-nodes[x].count;nodes[p].child[1]=x;nodes[x].side=1;rotate(x);pull(x);}}
    void make_root(u32 v){access(v+1);reverse(v+1);}
    void link(u32 u,u32 v){make_root(u);u32 x=u+1,y=v+1;access(y);apply(x,-nodes[y].add);nodes[x].parent=y;nodes[y].local+=nodes[x].sum;nodes[y].virtual_count+=nodes[x].count;pull(y);}
    void cut(u32 u,u32 v){make_root(u);u32 x=u+1,y=v+1;access(y);apply(x,nodes[y].add);nodes[x].parent=0;nodes[x].side=-1;nodes[y].child[0]=0;pull(y);}
    void add(u32 v,T delta){u32 x=v+1;access(x);nodes[x].local+=delta;pull(x);}
    T side_sum(u32 v,u32 p){u32 x=v+1,y=p+1;access(x);splay(y);pull(y);return nodes[y].parent==x?nodes[x].sum-nodes[y].sum-nodes[x].add*nodes[y].count:nodes[x].sum+nodes[y].add*nodes[x].count;}
    void side_add(u32 v,u32 p,T delta){u32 x=v+1,y=p+1;access(x);splay(y);pull(y);
        if(nodes[y].parent==x){apply(x,delta);apply(y,-delta);nodes[x].local-=delta*nodes[y].count;nodes[x].sum-=delta*nodes[y].count;}
        else{apply(x,delta);pull(y);}}
};
}
