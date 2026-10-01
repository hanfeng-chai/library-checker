#pragma once
#include <toy/buffer.h>
#include <toy/mod.h>
namespace toy {
// Maximum clique in a graph with at most 64 vertices. Greedy coloring bounds
// the extension size at every node of the branch-and-bound search.
inline u64 maximum_clique(std::span<const u64> adjacent){
    u32 n=adjacent.size(),best_size=0;u64 best=0;
    auto dfs=[&](auto&& self,u64 candidates,u64 chosen,u32 size)->void{
        if(!candidates){if(size>best_size){best_size=size;best=chosen;}return;}
        u8 order[64],bound[64];u32 count=0,color=0;u64 remaining=candidates;
        while(remaining){++color;u64 free=remaining;while(free){u32 v=std::countr_zero(free);u64 bit=1ull<<v;remaining^=bit;free&=~(bit|adjacent[v]);order[count]=v;bound[count++]=color;}}
        while(count){--count;if(size+bound[count]<=best_size)return;u32 v=order[count];u64 bit=1ull<<v;self(self,candidates&adjacent[v],chosen|bit,size+1);candidates^=bit;}
    };
    dfs(dfs,n==64?~0ull:(1ull<<n)-1,0,0);return best;
}
// Classic meet-in-the-middle independent set; at most 40 vertices.
inline u64 independent_set_mitm(std::span<const u64> adjacent){
    u32 n=adjacent.size(),l=n/2,r=n-l,limit=1u<<r;Buffer<u32>best(limit);best[0]=0;
    for(u32 mask=1;mask<limit;++mask){u32 v=std::countr_zero(mask),rest=mask&(mask-1),a=best[rest],b=best[rest&~u32(adjacent[l+v]>>l)]|(1u<<v);best[mask]=std::popcount(a)>=std::popcount(b)?a:b;}
    u64 answer=u64(best[limit-1])<<l;u32 size=std::popcount(answer);
    auto dfs=[&](auto&&self,u32 candidates,u32 allowed,u64 chosen,u32 count)->void{
        u32 b=best[allowed],total=count+std::popcount(b);if(total>size){size=total;answer=chosen|(u64(b)<<l);}
        while(candidates){if(count+std::popcount(candidates)+std::popcount(b)<=size)break;u32 v=std::countr_zero(candidates);candidates&=candidates-1;self(self,candidates&~u32(adjacent[v]),allowed&~u32(adjacent[v]>>l),chosen|(1ull<<v),count+1);}
    };
    dfs(dfs,(1u<<l)-1,limit-1,0,0);return answer;
}
// Sum of products of vertex weights over all nonempty cliques, n <= 128.
template<u32 P=998244353>u32 clique_sum(std::span<const u128> adjacent,std::span<const u32> weight){
    auto dfs=[&](auto&&self,u128 candidates)->u32{
        u32 total=1;while(candidates){u64 low=candidates;u32 v=low?std::countr_zero(low):64+std::countr_zero(u64(candidates>>64));candidates&=candidates-1;
            total=Mod<P>::add(total,Mod<P>::mul(weight[v],self(self,candidates&adjacent[v])));}
        return total;
    };
    u32 n=adjacent.size();return Mod<P>::sub(dfs(dfs,n==128?~u128(0):(u128(1)<<n)-1),1);
}
}
