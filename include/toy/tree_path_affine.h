#pragma once
#include <toy/heavy_light.h>
#include <toy/path_affine.h>
#include <toy/weighted_path_affine.h>
namespace toy {
template<u32 P=998244353>struct ParentPathAffine {
    using R=Montgomery<P>;struct Node{u32 parent,depth,a,b;};Buffer<Node>nodes;mutable Buffer<u32>pending;
    ParentPathAffine(const HeavyLight& h,std::span<const Affine<P>>values):nodes(values.size()){
        u32 height=0;for(u32 v=0;v<values.size();++v){nodes[v]={h.parent[v],h.depth[v],R::encode(values[v].a),values[v].b};height=std::max(height,h.depth[v]);}pending=Buffer<u32>(height+1);}
    void set(u32 v,Affine<P> f){nodes[v].a=R::encode(f.a);nodes[v].b=f.b;}
    u32 apply(u32 u,u32 v,u32 x)const{u32 count=0;while(u!=v){if(nodes[u].depth>=nodes[v].depth){x=R::add(R::multiply(nodes[u].a,x),nodes[u].b);u=nodes[u].parent;}else{pending[count++]=v;v=nodes[v].parent;}}
        x=R::add(R::multiply(nodes[u].a,x),nodes[u].b);while(count){u32 w=pending[--count];x=R::add(R::multiply(nodes[w].a,x),nodes[w].b);}return std::min(x,x-P);}
};
template<u32 P=998244353>struct HeavyPathAffine {
    Buffer<u32>position,head,parent;PathAffineTree<P>tree;
    HeavyPathAffine(const HeavyLight& h,std::span<const Affine<P>>values):position(values.size()),head(values.size()+1),parent(values.size()+1),tree([&]{Buffer<Affine<P>>a(values.size());for(u32 v=0;v<values.size();++v)a[h.position[v]]=values[v];return PathAffineTree<P>(std::span<const Affine<P>>(a));}()){
        for(u32 v=0;v<values.size();++v){u32 i=position[v]=h.position[v]+1;head[i]=h.position[h.head[v]]+1;parent[i]=h.position[h.parent[v]]+1;}}
    void set(u32 v,Affine<P> f){tree.set(position[v]-1,f);}
    u32 apply(u32 u,u32 v,u32 x)const{u=position[u];v=position[v];std::array<std::array<u32,2>,64>pending;u32 count=0;
        while(head[u]!=head[v]){if(head[u]>head[v]){x=tree.template apply<true>(head[u]-1,u,x);u=parent[head[u]];}else{pending[count++]={head[v]-1,v};v=parent[head[v]];}}
        x=u>v?tree.template apply<true>(v-1,u,x):tree.apply(u-1,v,x);while(count){auto[l,r]=pending[--count];x=tree.apply(l,r,x);}return x;}
};
inline u32 affine_path_method(const HeavyLight& h){u32 n=h.parent.n,height=0;for(u32 v=0;v<n;++v)height=std::max(height,h.depth[v]);if(height<=64)return 0;
    Buffer<u32>length(n),cost(n);std::fill(length.p,length.p+n,0u);for(u32 v=0;v<n;++v)++length[h.head[v]];u32 work=0;
    for(u32 i=0;i<n;++i){u32 v=h.vertex[i];cost[v]=(i?cost[h.parent[v]]:0)+(h.head[v]==v?std::bit_width(length[v]):0);work=std::max(work,cost[v]);}return work<=2*std::bit_width(n)?1:2;}
}
