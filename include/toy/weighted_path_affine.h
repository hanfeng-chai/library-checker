#pragma once
#include <toy/affine.h>
#include <toy/heavy_light.h>
#include <toy/montgomery.h>
namespace toy {
template<u32 P=998244353> struct WeightedPathAffine {
    using R=Montgomery<P>;
    struct Segment{u32 a=R::one,forward=0,reverse=0;};
    struct Node{u32 a=R::one,b=0;Segment sum,prefix;u32 left=0,right=0,parent=0;};
    Buffer<Node> nodes;Buffer<u8> priority;Buffer<u32>position,chain,up;
    static Segment combine(Segment a,Segment b){return {R::multiply(a.a,b.a),R::add(R::multiply(a.forward,b.a),b.forward),R::add(R::multiply(b.reverse,a.a),a.reverse)};}
    Segment value(u32 i)const{const auto& x=nodes[i];return {x.a,x.b,x.b};}
    void pull(u32 i){auto& x=nodes[i];x.prefix=combine(nodes[x.left].sum,value(i));x.sum=combine(x.prefix,nodes[x.right].sum);}
    WeightedPathAffine(const HeavyLight& h,std::span<const Affine<P>> values):nodes(values.size()+1),priority(values.size()+1),position(values.size()),chain(values.size()+1),up(values.size()+1){
        std::fill(nodes.p,nodes.p+nodes.n,Node{});u32 n=values.size();Buffer<u32>stack(n),order(n);u32 roots=0;
        for(u32 v=0;v<n;++v){u32 i=position[v]=h.position[v]+1;chain[i]=h.position[h.head[v]]+1;up[i]=h.position[h.parent[v]]+1;nodes[i].a=R::encode(values[v].a);nodes[i].b=values[v].b;}
        for(u32 head=0;head<n;++head)if(h.head[head]==head){u32 count=0,start=1;
            for(u32 v=head;v!=~0u;v=h.heavy[v]){u32 i=h.position[v]+1,weight=h.size[v]-(h.heavy[v]==~0u?0:h.size[h.heavy[v]]),last=0;
                // Dyadic priority of the vertex plus its light subtrees gives a weight-biased chain tree.
                priority[i]=std::bit_width(start^(start+weight))-1;start+=weight;
                while(count&&priority[stack[count-1]]<priority[i])last=stack[--count];
                if(count){nodes[stack[count-1]].right=i;nodes[i].parent=stack[count-1];}nodes[i].left=last;if(last)nodes[last].parent=i;stack[count++]=i;
            }order[roots++]=stack[0];}
        u32 count=roots;for(u32 i=0;i<count;++i){auto x=nodes[order[i]];if(x.left)order[count++]=x.left;if(x.right)order[count++]=x.right;}for(u32 i=count;i--;)pull(order[i]);
    }
    void set(u32 vertex,Affine<P> f){u32 i=position[vertex];nodes[i].a=R::encode(f.a);nodes[i].b=f.b;for(;i;i=nodes[i].parent)pull(i);}
    template<bool Reverse> static u32 map(Segment f,u32 x){return R::add(R::multiply(f.a,x),Reverse?f.reverse:f.forward);}
    u32 map_value(u32 i,u32 x)const{return R::add(R::multiply(nodes[i].a,x),nodes[i].b);}
    template<bool Reverse> u32 prefix(u32 vertex,u32 x)const{u32 last=vertex,count=0;std::array<u32,64>pending;
        for(u32 i=last;i;i=nodes[i].parent)if(i<=last){if constexpr(Reverse)x=map<true>(nodes[i].prefix,x);else pending[count++]=i;}
        if constexpr(!Reverse)while(count)x=map<false>(nodes[pending[--count]].prefix,x);return x;
    }
    template<bool Reverse> u32 interval(u32 left,u32 right,u32 x)const{u32 l=left,r=right,u=l,v=r,count=0;std::array<u32,64>pending;
        while(u!=v){if(priority[u]<priority[v]){if(u>=l){if constexpr(Reverse)pending[count++]=u;else{x=map_value(u,x);if(nodes[u].right)x=map<false>(nodes[nodes[u].right].sum,x);}}u=nodes[u].parent;}
            else{if(v<=r){if constexpr(Reverse)x=map<true>(nodes[v].prefix,x);else pending[count++]=v;}v=nodes[v].parent;}}
        x=map_value(u,x);while(count){u32 i=pending[--count];if constexpr(Reverse){if(nodes[i].right)x=map<true>(nodes[nodes[i].right].sum,x);x=map_value(i,x);}else x=map<false>(nodes[i].prefix,x);}return x;
    }
    u32 apply(u32 u,u32 v,u32 x)const{u=position[u];v=position[v];std::array<u32,64>pending;u32 count=0;
        while(chain[u]!=chain[v]){if(chain[u]>chain[v]){x=prefix<true>(u,x);u=up[chain[u]];}else{pending[count++]=v;v=up[chain[v]];}}
        x=u>v?interval<true>(v,u,x):interval<false>(u,v,x);
        while(count)x=prefix<false>(pending[--count],x);return std::min(x,x-P);
    }
};
}
