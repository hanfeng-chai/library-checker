#pragma once
#include <toy/buffer.h>
namespace toy {
// Network simplex for bounded circulation. An artificial root supplies the
// initial feasible tree; a penalty above every real simple-path cost removes
// artificial flow exactly when the original problem is feasible.
struct NetworkSimplex {
    struct Arc{u32 from,to;i64 capacity,cost;};
    u32 n,m,used=0,version=1;Buffer<Arc>arc;Buffer<i64>balance,lower,potential,flow;Buffer<u32>parent,tree_edge,mark;Buffer<u128>subtree;i128 cost=0;
    NetworkSimplex(u32 n,u32 m):n(n),m(m),arc(2*(m+n)),balance(n),lower(m),potential(n+1),flow(m),parent(n+1),tree_edge(n),mark(n+1),subtree(n<=128?n+1:0){std::fill(balance.p,balance.p+n,0ll);std::fill(mark.p,mark.p+n+1,0u);potential[n]=0;parent[n]=n;}
    u32 link(u32 u,u32 v,i64 capacity,i64 c){u32 id=used;arc[used++]={u,v,capacity,c};arc[used++]={v,u,0,-c};return id;}
    void add(u32 u,u32 v,i64 lo,i64 hi,i64 c){lower[used/2]=lo;balance[u]-=lo;balance[v]+=lo;link(u,v,hi-lo,c);}
    i64 price(u32 v){if(subtree.n)return potential[v];if(v==n)return 0;if(mark[v]!=version){mark[v]=version;potential[v]=price(parent[v])+arc[tree_edge[v]].cost;}return potential[v];}
    i64 pivot(u32 entering){u32 u=arc[entering].from,v=arc[entering].to,common=v;++version;for(u32 x=u;;x=parent[x]){mark[x]=version;if(x==n)break;}while(mark[common]!=version){mark[common]=version;common=parent[common];}
        i64 amount=arc[entering].capacity;u32 leaving=entering,child=~0u,side=0;
        auto consider=[&](u32 x,u32 e,u32 s){i64 c=arc[e].capacity;if(c<amount||(c==amount&&(e/2)<leaving/2)){amount=c;leaving=e;child=x;side=s;}};
        for(u32 x=u;x!=common;x=parent[x])consider(x,tree_edge[x],0);for(u32 x=v;x!=common;x=parent[x])consider(x,tree_edge[x]^1,1);
        auto push=[&](u32 e){arc[e].capacity-=amount;arc[e^1].capacity+=amount;};push(entering);for(u32 x=u;x!=common;x=parent[x])push(tree_edge[x]);for(u32 x=v;x!=common;x=parent[x])push(tree_edge[x]^1);
        if(child==~0u)return amount;u128 moved=0,prefix=0;
        if(subtree.n){moved=subtree[child];i64 change=arc[entering].cost+potential[u]-potential[v];if(!side)change=-change;
            for(u128 bits=moved;bits;bits&=bits-1){u64 low=bits;u32 x=low?std::countr_zero(low):64+std::countr_zero(u64(bits>>64));potential[x]+=change;}
            for(u32 x=parent[child];;x=parent[x]){subtree[x]^=moved;if(x==n)break;}
            for(u32 x=side?u:v;;x=parent[x]){subtree[x]^=moved;if(x==n)break;}
        }
        if(side)std::swap(u,v);u32 previous=v,edge=entering^side;
        // Reverse the tree path up to the leaving edge, reconnecting its
        // detached subtree through the entering edge.
        while(previous!=child){if(subtree.n){u128 old=subtree[u];subtree[u]=moved^prefix;prefix=old;}--mark[u];edge^=1;std::swap(tree_edge[u],edge);u32 next=parent[u];parent[u]=previous;previous=u;u=next;}return amount;
    }
    bool solve(){i64 total=0,maximum=0;for(u32 v=0;v<n;++v)total+=balance[v];if(total)return false;for(u32 e=0;e<2*m;e+=2)maximum=std::max(maximum,std::abs(arc[e].cost));i64 penalty=maximum*(n+1)+1,infinity=1ll<<60;
        for(u32 v=0;v<n;++v){bool supply=balance[v]>=0;u32 e=supply?link(v,n,infinity,penalty):link(n,v,infinity,penalty);i64 amount=std::abs(balance[v]);arc[e].capacity-=amount;arc[e^1].capacity=amount;parent[v]=n;tree_edge[v]=e^u32(supply);if(subtree.n){subtree[v]=u128(1)<<v;potential[v]=arc[tree_edge[v]].cost;}}
        if(subtree.n)subtree[n]=n==128?~u128(0):(u128(1)<<n)-1;
        // Block pricing avoids the tiny improvements of first-negative scans.
        // After n zero-flow pivots, Bland's rule prevents a degenerate cycle;
        // return to block pricing as soon as the objective strictly decreases.
        u32 cursor=0,block=8,zeros=0;
        for(;;){u32 best=~0u;
            if(zeros>=n){for(u32 e=0;e<used;++e)if(arc[e].capacity&&arc[e].cost+price(arc[e].from)-price(arc[e].to)<0){best=e;break;}}
            else for(u32 scanned=0;scanned<used&&best==~0u;){i64 minimum=0;u32 count=std::min(block,used-scanned);scanned+=count;
                while(count--){u32 e=cursor;if(++cursor==used)cursor=0;if(!arc[e].capacity)continue;i64 value=arc[e].cost+price(arc[e].from)-price(arc[e].to);if(value<minimum){minimum=value;best=e;}}
            }
            if(best==~0u)break;if(pivot(best))zeros=0;else ++zeros;
        }
        for(u32 i=0;i<n;++i)if(arc[2*(m+i)+1].capacity)return false;
        for(u32 v=0;v<n;++v)price(v);for(u32 i=0;i<m;++i){flow[i]=lower[i]+arc[2*i+1].capacity;cost+=i128(flow[i])*arc[2*i].cost;}return true;
    }
};
}
