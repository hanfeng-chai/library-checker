#pragma once
#include <toy/adjacency.h>
#include <toy/affine.h>
#include <toy/montgomery.h>
namespace toy {
template<u32 P=998244353> struct LinkCutAffine {
    using R=Montgomery<P>;
    struct Node{u32 child[2]{},parent=0;i8 side=-1;bool reversed=false;u32 a=R::one,b=0,total_a=R::one,forward=0,backward=0;};
    Buffer<Node> nodes;
    explicit LinkCutAffine(std::span<const Affine<P>> values):nodes(values.size()+1){std::fill(nodes.p,nodes.p+nodes.n,Node{});for(u32 i=0;i<values.size();++i){auto& x=nodes[i+1];x.a=x.total_a=R::encode(values[i].a);x.b=x.forward=x.backward=values[i].b;}}
    void build_tree(const Adjacency& graph,u32 root=0){u32 n=graph.size();if(!n)return;Buffer<u32>order(n);order[0]=root+1;for(u32 i=0,count=1;i<count;++i){u32 v=order[i];for(u32 w:graph[v-1])if(w+1!=nodes[v].parent){nodes[w+1].parent=v;order[count++]=w+1;}}}
    void pull(u32 i){auto& x=nodes[i];const auto& l=nodes[x.child[0]];const auto& r=nodes[x.child[1]];
        u32 a=R::multiply(l.total_a,x.a),forward=R::add(R::multiply(l.forward,x.a),x.b),backward=R::add(R::multiply(x.b,l.total_a),l.backward);
        x.total_a=R::multiply(a,r.total_a);x.forward=R::add(R::multiply(forward,r.total_a),r.forward);x.backward=R::add(R::multiply(r.backward,a),backward);}
    void reverse(u32 x){std::swap(nodes[x].forward,nodes[x].backward);nodes[x].reversed^=1;}
    void push(u32 x){auto& v=nodes[x];if(v.reversed){std::swap(v.child[0],v.child[1]);nodes[v.child[0]].side=0;nodes[v.child[1]].side=1;reverse(v.child[0]);reverse(v.child[1]);v.reversed=false;}}
    void rotate(u32 x){u32 y=nodes[x].parent,z=nodes[y].parent,s=nodes[x].side,m=nodes[x].child[s^1];i8 t=nodes[y].side;nodes[m].parent=y;nodes[m].side=s;nodes[y].child[s]=m;nodes[y].parent=x;nodes[y].side=s^1;nodes[x].child[s^1]=y;nodes[x].parent=z;nodes[x].side=t;if(t>=0)nodes[z].child[t]=x;pull(y);}
    void splay(u32 x){push(x);while(nodes[x].side>=0){u32 y=nodes[x].parent,z=nodes[y].parent;if(nodes[y].side>=0){push(z);push(y);push(x);rotate(nodes[x].side==nodes[y].side?y:x);}else{push(y);push(x);}rotate(x);}}
    void access(u32 x){splay(x);nodes[nodes[x].child[1]].side=-1;nodes[x].child[1]=0;while(u32 p=nodes[x].parent){splay(p);nodes[nodes[p].child[1]].side=-1;nodes[p].child[1]=x;nodes[x].side=1;rotate(x);}pull(x);}
    void make_root(u32 vertex){u32 x=vertex+1;access(x);reverse(x);}
    void link(u32 u,u32 v){make_root(u);nodes[u+1].parent=v+1;}
    void cut(u32 u,u32 v){make_root(u);u32 x=u+1,y=v+1;access(y);nodes[y].child[0]=0;nodes[x].parent=0;nodes[x].side=-1;pull(y);}
    void set(u32 vertex,Affine<P> f){u32 x=vertex+1;splay(x);nodes[x].a=R::encode(f.a);nodes[x].b=f.b;pull(x);}
    u32 apply(u32 u,u32 v,u32 value){make_root(u);access(v+1);const auto& x=nodes[v+1];u32 answer=R::add(R::multiply(x.total_a,value),x.forward);return std::min(answer,answer-P);}
};
}
