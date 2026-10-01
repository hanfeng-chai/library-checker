#pragma once
#include <toy/affine.h>
#include <toy/buffer.h>
#include <toy/montgomery.h>
namespace toy {
template<u32 P=998244353> struct PathAffineTree {
    using R=Montgomery<P>;struct Node{u32 a=R::one,forward=0,reverse=0;};
    u32 n;Buffer<Node> tree;
    static Node combine(Node a,Node b){return {R::multiply(a.a,b.a),R::add(R::multiply(a.forward,b.a),b.forward),R::add(R::multiply(b.reverse,a.a),a.reverse)};}
    explicit PathAffineTree(std::span<const Affine<P>> values):n(values.size()),tree(2*std::max(1u,n)){tree[0]={};for(u32 i=0;i<n;++i)tree[n+i]={R::encode(values[i].a),values[i].b,values[i].b};for(u32 i=n;i-->1;)tree[i]=combine(tree[2*i],tree[2*i+1]);}
    void set(u32 i,Affine<P> f){tree[i+=n]={R::encode(f.a),f.b,f.b};while(i>>=1)tree[i]=combine(tree[2*i],tree[2*i+1]);}
    template<bool Reverse=false> u32 apply(u32 l,u32 r,u32 x)const{
        std::array<u32,32> pending;u32 count=0;
        auto map=[&](u32 i){auto f=tree[i];x=R::add(R::multiply(f.a,x),Reverse?f.reverse:f.forward);};
        for(l+=n,r+=n;l<r;l>>=1,r>>=1){if(l&1){if constexpr(Reverse)pending[count++]=l;else map(l);++l;}if(r&1){--r;if constexpr(Reverse)map(r);else pending[count++]=r;}}
        while(count)map(pending[--count]);return std::min(x,x-P);
    }
};
}
