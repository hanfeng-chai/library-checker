#pragma once
#include <toy/dynamic_component_sum.h>
namespace toy {
// Maximum-bottleneck AM forest labelled by future deletion times. Expiring
// the globally earliest label never requires searching for a replacement edge.
struct ExpiryForest {
    struct Node{u64 sum=0;u32 parent=0,deadline=0,size=1;};
    Buffer<Node>node;Buffer<u32>owner;u32 horizon;
    ExpiryForest(std::span<const u64>values,u32 q):node(values.size()+1),owner(q),horizon(q){std::fill(node.p,node.p+node.n,Node{});std::fill(owner.p,owner.p+q,0u);node[0].size=0;for(u32 i=0;i<values.size();++i)node[i+1].sum=values[i];}
    void unindex(u32 x){u32 d=node[x].deadline;if(d&&d<horizon)owner[d]=0;}
    void index(u32 x){u32 d=node[x].deadline;if(d&&d<horizon)owner[d]=x;}
    void promote(u32 x){auto&a=node[x];u32 y=a.parent;auto&b=node[y];b.size-=a.size;b.sum-=a.sum;
        if(a.deadline<=b.deadline)a.parent=b.parent;
        else{a.size+=b.size;a.sum+=b.sum;unindex(x);unindex(y);std::swap(a.deadline,b.deadline);a.parent=b.parent;b.parent=x;index(x);index(y);}
    }
    void maintain(u32 x){while(u32 p=node[x].parent){if(u64(node[x].size)*3<=u64(node[p].size)*2)x=p;else promote(x);}}
    void perch(u32 x,u32 stop){while(node[x].parent&&node[x].parent!=stop)promote(x);}
    void insert(u32 u,u32 v,u32 deadline){if(u==v)return;++u;++v;maintain(u);maintain(v);perch(v,0);perch(u,v);
        if(node[u].parent==v){if(deadline>node[u].deadline){unindex(u);node[u].deadline=deadline;index(u);}}
        else{if(node[u].size>node[v].size)std::swap(u,v);node[u].parent=v;node[u].deadline=deadline;index(u);node[v].size+=node[u].size;node[v].sum+=node[u].sum;}
    }
    void expire(u32 time){u32 x=owner[time];if(!x)return;u32 count=node[x].size;u64 sum=node[x].sum;for(u32 p=node[x].parent;p;p=node[p].parent){node[p].size-=count;node[p].sum-=sum;}node[x].parent=node[x].deadline=owner[time]=0;}
    void add(u32 v,u64 delta){++v;maintain(v);for(;v;v=node[v].parent)node[v].sum+=delta;}
    u64 sum(u32 v){++v;maintain(v);while(node[v].parent)v=node[v].parent;return node[v].sum;}
};
inline Buffer<u64> expiry_component_sum(std::span<const u64>initial,std::span<const GraphOperation>operations){
    u32 q=operations.size(),insertions=0;bool deletion=false;for(auto o:operations){insertions+=o.type==0;deletion|=o.type==1;}if(!deletion)return dynamic_component_sum(initial,operations);
    Buffer<u32>death(q);std::fill(death.p,death.p+q,q);HashMap<u64,u32>active(insertions);auto key=[](u32 u,u32 v){return (u64(std::min(u,v))<<32)|std::max(u,v);};
    for(u32 t=0;t<q;++t){auto o=operations[t];if(o.type==0)active[key(o.u,o.v)]=t;else if(o.type==1)death[active[key(o.u,o.v)]]=t;}
    ExpiryForest forest(initial,q);Buffer<u64>answer(0,q);for(u32 t=0;t<q;++t){auto o=operations[t];if(o.type==0)forest.insert(o.u,o.v,death[t]);else if(o.type==1)forest.expire(t);else if(o.type==2)forest.add(o.u,o.v);else answer.p[answer.n++]=forest.sum(o.u);}return answer;
}
}
