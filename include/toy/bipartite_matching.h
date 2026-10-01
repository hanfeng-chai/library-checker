#pragma once
#include <toy/adjacency.h>
namespace toy {
// Push-relabel matching, with global distance labels rebuilt every O(n) pushes.
struct BipartitePush {
    Buffer<u32>left,right;u32 size=0;
    BipartitePush(u32 l,u32 r,std::span<const std::array<u32,2>>input):left(l),right(r){
        u32 n=l+r;Buffer<std::array<u32,2>>edges(input.size());for(u32 i=0;i<input.size();++i)edges[i]={input[i][0],input[i][1]+l};Adjacency g(n,edges);edges={};
        Buffer<u32>match(n),distance(n),queue(n),active(r+1);std::fill(match.p,match.p+n,~0u);
        for(u32 u=0;u<l;++u)for(u32 v:g[u])if(match[v]==~0u){match[v]=u;match[u]=v;++size;break;}
        u32 begin=0,end=0;for(u32 v=l;v<n;++v)if(match[v]==~0u)active[end++]=v;
        for(u32 pushes=0;begin!=end;){
            if(!pushes){std::fill(distance.p,distance.p+n,n);u32 head=0,tail=0;for(u32 u=0;u<l;++u)if(match[u]==~0u){distance[u]=0;queue[tail++]=u;}
                while(head<tail){u32 u=queue[head++];for(u32 v:g[u])if(distance[v]>distance[u]+1){distance[v]=distance[u]+1;u32 w=match[v];if(w!=~0u){distance[w]=distance[v]+1;queue[tail++]=w;}}}
            }
            u32 v=active[begin];if(++begin==active.n)begin=0;u32 best=~0u,d=n;
            for(u32 u:g[v])if(distance[u]<d){best=u;d=distance[u];}
            if(best!=~0u){u32 old=match[best];if(old!=~0u){match[old]=~0u;active[end]=old;if(++end==active.n)end=0;}else ++size;
                match[v]=best;match[best]=v;distance[v]=d+1;distance[best]=d+2;
            }
            if(++pushes==n)pushes=0;
        }
        for(u32 u=0;u<l;++u)left[u]=match[u]==~0u?~0u:match[u]-l;for(u32 v=0;v<r;++v)right[v]=match[l+v];
    }
};
// Hopcroft-Karp on CSR rows. Explicit augmenting stacks avoid recursion limits.
struct BipartiteMatching {
    Buffer<u32> left,right;u32 size=0;
    BipartiteMatching(u32 l,u32 r,Buffer<std::array<u32,2>> edges):left(std::min(l,r)),right(std::max(l,r)){
        bool flip=l>r;if(flip){std::swap(l,r);for(auto& e:std::span(edges.p,edges.n))std::swap(e[0],e[1]);}
        Adjacency g(l,edges,true);edges={};std::fill(left.p,left.p+l,~0u);std::fill(right.p,right.p+r,~0u);
        // Seed degree-one rows first, then a greedy matching for the remaining rows.
        for(int pass=0;pass<2;++pass)for(u32 u=0;u<l;++u)if(left[u]==~0u&&(pass||g[u].size()==1))for(u32 v:g[u])if(right[v]==~0u){left[u]=v;right[v]=u;++size;break;}
        if(size<l){Buffer<u32>level(l),queue(l),cursor(l),path(l);
            while(size<l){
                std::fill(level.p,level.p+l,~0u);u32 head=0,tail=0,limit=~0u;
                for(u32 u=0;u<l;++u)if(left[u]==~0u){level[u]=0;queue[tail++]=u;}
                while(head<tail){u32 u=queue[head++];if(level[u]>=limit)continue;
                    for(u32 v:g[u]){u32 mate=right[v];if(mate==~0u)limit=level[u]+1;else if(level[mate]==~0u){level[mate]=level[u]+1;queue[tail++]=mate;}}
                }
                if(limit==~0u)break;memcpy(cursor.p,g.offset.p,l*4);
                for(u32 root=0;root<l;++root)if(left[root]==~0u){
                    u32 depth=1;path[0]=root;
                    while(depth){u32 u=path[depth-1];auto& at=cursor[u];
                        if(at==g.offset[u+1]){level[u]=~0u;--depth;continue;}
                        u32 v=g.edge[at++],mate=right[v];
                        if(mate==~0u){if(level[u]+1!=limit)continue;
                            while(depth){u32 x=path[--depth],old=left[x];left[x]=v;right[v]=x;v=old;}++size;break;
                        }
                        if(level[mate]==level[u]+1)path[depth++]=mate;
                    }
                }
            }
        }
        if(flip)std::swap(left,right);
    }
};
}
