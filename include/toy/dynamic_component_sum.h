#pragma once
#include <toy/hash.h>
#include <toy/dsu.h>
namespace toy {
struct GraphOperation{u32 type,u,v;};
// Edge lifetimes and persistent vertex additions become time intervals.
// Rollback union-find evaluates them on a segment tree over query positions.
inline Buffer<u64> dynamic_component_sum(std::span<const u64>initial,std::span<const GraphOperation>operations){
    u32 n=initial.size(),q=operations.size();Buffer<u64>answer(0,q),sum(n);memcpy(sum.p,initial.data(),n*8);DSU dsu(n);bool deletion=false;for(auto o:operations)deletion|=o.type==1;
    if(!deletion){for(auto o:operations){u32 u=dsu.leader(o.u);if(o.type==0){u32 v=dsu.leader(o.v);if(u!=v){u64 value=sum[u]+sum[v];dsu.merge(u,v);sum[dsu.leader(u)]=value;}}else if(o.type==2)sum[u]+=o.v;else if(o.type==3)answer.p[answer.n++]=sum[u];}return answer;}
    struct Interval{u32 l,r,u,v;};Buffer<Interval>interval(0,q);HashMap<u64,u32>active(q);
    auto key=[](u32 u,u32 v){if(u>v)std::swap(u,v);return (u64(u)<<32)|v;};
    for(u32 t=0;t<q;++t){auto o=operations[t];if(o.type==0)active[key(o.u,o.v)]=t+1;else if(o.type==1){auto& start=active[key(o.u,o.v)];interval.p[interval.n++]={start-1,t,o.u,o.v};start=0;}else if(o.type==2)interval.p[interval.n++]={t,q,o.u|(1u<<31),o.v};}
    for(auto e:std::span(active.table.p,active.table.n))if(e.key!=~0ull&&e.value)interval.p[interval.n++]={e.value-1,q,u32(e.key>>32),u32(e.key)};
    u32 base=std::bit_ceil(std::max(q,1u));Buffer<u32>offset(2*base+1);std::fill(offset.p,offset.p+offset.n,0u);
    auto cover=[&](Interval x,auto f){for(u32 l=x.l+base,r=x.r+base;l<r;l/=2,r/=2){if(l&1)f(l++);if(r&1)f(--r);}};
    for(auto x:std::span(interval.p,interval.n))cover(x,[&](u32 v){++offset[v+1];});for(u32 i=1;i<offset.n;++i)offset[i]+=offset[i-1];Buffer<std::array<u32,2>>events(offset[2*base]);Buffer<u32>cursor(offset.n);memcpy(cursor.p,offset.p,offset.n*4);
    for(auto x:std::span(interval.p,interval.n))cover(x,[&](u32 v){events[cursor[v]++]={x.u,x.v};});interval={};cursor={};
    struct Change{u32 root,child;i32 size;u64 sum;};Buffer<Change>history(0,2*q+1);
    auto leader=[&](u32 v){while(dsu.parent[v]>=0)v=dsu.parent[v];return v;};
    auto dfs=[&](auto&&self,u32 node)->void{u32 checkpoint=history.n;
        for(u32 i=offset[node];i<offset[node+1];++i){auto[u,v]=events[i];if(u>>31){u=leader(u&0x7fffffff);history.p[history.n++]={u,~0u,0,sum[u]};sum[u]+=v;}
            else{u=leader(u);v=leader(v);if(u==v)continue;if(dsu.parent[u]>dsu.parent[v])std::swap(u,v);history.p[history.n++]={u,v,dsu.parent[v],sum[u]};dsu.parent[u]+=dsu.parent[v];dsu.parent[v]=u;sum[u]+=sum[v];}
        }
        if(node>=base){u32 t=node-base;if(t<q&&operations[t].type==3)answer.p[answer.n++]=sum[leader(operations[t].u)];}
        else{self(self,node*2);self(self,node*2+1);}
        while(history.n>checkpoint){auto x=history[--history.n];sum[x.root]=x.sum;if(x.child!=~0u){dsu.parent[x.root]-=x.size;dsu.parent[x.child]=x.size;}}
    };dfs(dfs,1);return answer;
}
}
