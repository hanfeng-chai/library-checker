#pragma once
#include <toy/adjacency.h>
namespace toy {
struct ChordalGraph {
    bool chordal=true;Buffer<u32>order,cycle;
    explicit ChordalGraph(const Adjacency& g):order(g.size()){
        u32 n=g.size(),peeled=0,end=0;Buffer<u32>degree(n),queue(n);Buffer<u8>removed(n);std::fill(removed.p,removed.p+n,0);
        for(u32 v=0;v<n;++v){degree[v]=g[v].size();if(degree[v]<=1)queue[end++]=v;}
        // Leaves are already simplicial. Only the remaining 2-core needs MCS.
        while(peeled<end){u32 v=queue[peeled];order[peeled++]=v;removed[v]=1;for(u32 w:g[v])if(!removed[w]&&--degree[w]==1)queue[end++]=w;}
        if(peeled==n)return;bool cycles=true;for(u32 v=0;v<n;++v)if(!removed[v]&&degree[v]!=2)cycles=false;
        if(cycles){u32 at=peeled;for(u32 root=0;root<n;++root)if(!removed[root]){u32 begin=at,v=root,previous=~0u;do{order[at++]=v;removed[v]=1;u32 next=~0u;for(u32 w:g[v])if(w!=previous&&(!removed[w]||w==root)){next=w;break;}previous=v;v=next;}while(v!=root);
                if(at-begin>=4){chordal=false;cycle=Buffer<u32>(at-begin);memcpy(cycle.p,order.p+begin,cycle.n*4);return;}}
            return;
        }
        Buffer<u32>head(n+1),next(n),previous(n),weight(n),rank(n);std::fill(head.p,head.p+n+1,~0u);std::fill(weight.p,weight.p+n,0u);std::fill(rank.p,rank.p+n,~0u);for(u32 i=0;i<peeled;++i)rank[order[i]]=i;
        auto insert=[&](u32 v,u32 w){previous[v]=~0u;next[v]=head[w];if(head[w]!=~0u)previous[head[w]]=v;head[w]=v;};
        auto remove=[&](u32 v){u32 a=previous[v],b=next[v];if(a==~0u)head[weight[v]]=b;else next[a]=b;if(b!=~0u)previous[b]=a;};
        for(u32 v=0;v<n;++v)if(!removed[v])insert(v,0);u32 top=0;
        for(u32 i=n;i-->peeled;){while(head[top]==~0u)--top;u32 v=head[top];remove(v);order[i]=v;rank[v]=i;
            for(u32 w:g[v])if(rank[w]==~0u){remove(w);insert(w,++weight[w]);top=std::max(top,weight[w]);}
        }
        // Group the "later neighbors form a clique" checks by their earliest
        // later neighbor. Marking one row per parent makes verification O(n+m).
        Buffer<u32>parent(n),first(n),link(n),mark(n),bad(n);std::fill(first.p,first.p+n,~0u);std::fill(mark.p,mark.p+n,~0u);std::fill(bad.p,bad.p+n,~0u);
        for(u32 v=0;v<n;++v)if(!removed[v]){u32 p=~0u;for(u32 w:g[v])if(rank[w]>rank[v]&&(p==~0u||rank[w]<rank[p]))p=w;parent[v]=p;if(p!=~0u){link[v]=first[p];first[p]=v;}}
        for(u32 p=0;p<n;++p)if(first[p]!=~0u){for(u32 w:g[p])mark[w]=p;for(u32 v=first[p];v!=~0u;v=link[v])for(u32 w:g[v])if(w!=p&&rank[w]>rank[v]&&mark[w]!=p){bad[v]=w;break;}}
        u32 v=~0u;for(u32 u:std::span(order.p,order.n))if(bad[u]!=~0u){v=u;break;}if(v==~0u)return;chordal=false;
        u32 source=parent[v],target=bad[v];std::fill(mark.p,mark.p+n,0u);for(u32 w:g[v])mark[w]=1;mark[v]=1;mark[source]=mark[target]=0;
        std::fill(previous.p,previous.p+n,~0u);previous[source]=source;queue[0]=source;u32 begin=0;end=1;
        while(begin<end&&previous[target]==~0u){u32 u=queue[begin++];for(u32 w:g[u])if(!mark[w]&&previous[w]==~0u){previous[w]=u;queue[end++]=w;}}
        cycle=Buffer<u32>(0,n);cycle.p[cycle.n++]=v;for(u32 u=target;;u=previous[u]){cycle.p[cycle.n++]=u;if(u==source)break;}
    }
};
}
