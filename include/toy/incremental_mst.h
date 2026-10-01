#pragma once
#include <toy/link_cut_sum.h>
#include <toy/dsu.h>
namespace toy {
struct PathMaximum {
    u64 key=0;
    friend PathMaximum operator+(PathMaximum a,PathMaximum b){return {std::max(a.key,b.key)};}
};
// Unique positive weights. Reuse one link-cut node per live forest edge.
struct IncrementalMST {
    DSU dsu;LinkCutSum<PathMaximum>tree;Buffer<std::array<u32,2>>ends;Buffer<u32>slot;u32 n,used=0;
    IncrementalMST(u32 n,u32 m):dsu(n),tree([&]{Buffer<PathMaximum>v(2*n);std::fill(v.p,v.p+v.n,PathMaximum{});return v;}()),ends(n),slot(m),n(n){}
    i32 add(u32 u,u32 v,u32 weight,u32 id){
        if(u==v)return id;u32 node;i32 removed=-1;
        if(dsu.merge(u,v))node=n+used++;
        else{u64 maximum=tree.path_sum(u,v).key;if((maximum>>32)<weight)return id;removed=u32(maximum)-1;node=slot[removed];auto[a,b]=ends[node-n];tree.cut(a,node);tree.cut(b,node);}
        u32 x=node+1;tree.nodes[x].value=tree.nodes[x].sum={(u64(weight)<<32)|(id+1)};ends[node-n]={u,v};slot[id]=node;tree.link(u,node);tree.link(v,node);return removed;
    }
};
}
