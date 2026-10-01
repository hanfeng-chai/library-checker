#pragma once
#include <toy/adjacency.h>
namespace toy {
template<class T=i64,bool Subtree=false> struct LinkCutSum {
    struct Extra{T virtual_sum{};};struct Empty{};
    struct Node{T value{},sum{};[[no_unique_address]]std::conditional_t<Subtree,Extra,Empty> extra;u32 child[2]{},parent=0;i8 side=-1;bool reversed=false;};
    Buffer<Node>nodes;
    explicit LinkCutSum(std::span<const T> values):nodes(values.size()+1){std::fill(nodes.p,nodes.p+nodes.n,Node{});for(u32 i=0;i<values.size();++i)nodes[i+1].value=nodes[i+1].sum=values[i];}
    void build_tree(const Adjacency& graph,u32 root=0){u32 n=graph.size();if(!n)return;Buffer<u32>order(n);order[0]=root+1;for(u32 i=0,count=1;i<count;++i){u32 v=order[i];for(u32 w:graph[v-1])if(w+1!=nodes[v].parent){nodes[w+1].parent=v;order[count++]=w+1;}}
        if constexpr(Subtree)for(u32 i=n;i-->1;){auto& x=nodes[order[i]];auto& p=nodes[x.parent];p.extra.virtual_sum+=x.sum;p.sum+=x.sum;}}
    void build_ordered_tree(std::span<const u32> parent){for(u32 v=1;v<parent.size();++v)nodes[v+1].parent=parent[v]+1;
        if constexpr(Subtree)for(u32 v=parent.size();v-->1;){auto& x=nodes[v+1];auto& p=nodes[x.parent];p.extra.virtual_sum+=x.sum;p.sum+=x.sum;}}
    void pull(u32 x){auto& v=nodes[x];v.sum=nodes[v.child[0]].sum+v.value+nodes[v.child[1]].sum;if constexpr(Subtree)v.sum+=v.extra.virtual_sum;}
    void reverse(u32 x){nodes[x].reversed^=1;}
    void push(u32 x){auto& v=nodes[x];if(v.reversed){std::swap(v.child[0],v.child[1]);nodes[v.child[0]].side=0;nodes[v.child[1]].side=1;reverse(v.child[0]);reverse(v.child[1]);v.reversed=false;}}
    void rotate(u32 x){u32 y=nodes[x].parent,z=nodes[y].parent,s=nodes[x].side,m=nodes[x].child[s^1];i8 t=nodes[y].side;
        nodes[m].parent=y;nodes[m].side=s;nodes[y].child[s]=m;nodes[y].parent=x;nodes[y].side=s^1;nodes[x].child[s^1]=y;nodes[x].parent=z;nodes[x].side=t;if(t>=0)nodes[z].child[t]=x;pull(y);}
    [[gnu::always_inline]] void splay(u32 x){push(x);while(nodes[x].side>=0){u32 y=nodes[x].parent,z=nodes[y].parent;if(nodes[y].side>=0){push(z);push(y);push(x);rotate(nodes[x].side==nodes[y].side?y:x);}else{push(y);push(x);}rotate(x);}}
    void access(u32 x){splay(x);u32 old=nodes[x].child[1];nodes[old].side=-1;if constexpr(Subtree)nodes[x].extra.virtual_sum+=nodes[old].sum;nodes[x].child[1]=0;if constexpr(Subtree)pull(x);
        while(u32 p=nodes[x].parent){splay(p);old=nodes[p].child[1];nodes[old].side=-1;if constexpr(Subtree)nodes[p].extra.virtual_sum+=nodes[old].sum-nodes[x].sum;nodes[p].child[1]=x;nodes[x].side=1;rotate(x);if constexpr(Subtree)pull(x);}

    }
    void make_root(u32 v){access(v+1);reverse(v+1);}
    void link(u32 u,u32 v){make_root(u);u32 x=u+1,y=v+1;if constexpr(Subtree)access(y);nodes[x].parent=y;if constexpr(Subtree){nodes[y].extra.virtual_sum+=nodes[x].sum;pull(y);}}
    void cut(u32 u,u32 v){make_root(u);u32 x=u+1,y=v+1;access(y);nodes[y].child[0]=0;nodes[x].parent=0;nodes[x].side=-1;if constexpr(Subtree)pull(y);}
    void add(u32 v,T delta){u32 x=v+1;if constexpr(Subtree)access(x);else splay(x);nodes[x].value+=delta;if constexpr(Subtree)pull(x);}
    T path_sum(u32 u,u32 v) requires(!Subtree){make_root(u);access(v+1);return nodes[nodes[v+1].child[0]].sum+nodes[v+1].value;}
    T side_sum(u32 v,u32 p) requires(Subtree){u32 x=v+1,y=p+1;access(x);splay(y);pull(y);return nodes[y].parent==x?nodes[x].sum-nodes[y].sum:nodes[x].sum;}
    T subtree_sum(u32 v,u32 root) requires(Subtree){make_root(root);access(v+1);return nodes[v+1].value+nodes[v+1].extra.virtual_sum;}
};
}
