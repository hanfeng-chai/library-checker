#pragma once
#include <toy/packed_digraph.h>
#include <toy/dsu.h>
#include <toy/mod.h>
namespace toy {
// Offline divide-and-conquer over insertion times. SCCs found at the midpoint
// are contracted in the right half; only internal edges recurse to the left.
inline Buffer<u32> scc_merge_times(u32 n,std::span<const std::array<u32,2>>input){
    u32 m=input.size();Buffer<u32>time(m),map(n);std::fill(time.p,time.p+m,m);std::fill(map.p,map.p+n,~0u);
    // In a functional digraph, every nontrivial SCC is one directed cycle.
    // Its merge time is the latest insertion on that cycle. Reversal also works.
    if(m<=n){using D=PackedDigraph;auto vertex=page_buffer<u64>(n);Buffer<u32>path(0,n);
        for(u32 reverse=0;reverse<2;++reverse){std::fill(vertex.p,vertex.p+n,0ull);bool functional=true;
            for(u32 i=0;i<m;++i){u32 u=input[i][reverse],v=input[i][reverse^1];if(vertex[u]){functional=false;break;}vertex[u]=D::pack(i+1,v);}
            if(!functional)continue;u32 clock=0;
            for(u32 root=0;root<n;++root)if(vertex[root]&&!D::c(vertex[root])){u32 v=root,first=clock+1;path.n=0;
                while(vertex[v]&&!D::c(vertex[v])){u64 x=vertex[v];path.p[path.n++]=D::a(x)-1;vertex[v]=x|(u64(++clock)<<42);v=D::b(x);}
                if(vertex[v]&&D::c(vertex[v])>=first){u32 begin=D::c(vertex[v])-first;if(path.n-begin>1){u32 last=0;for(u32 i=begin;i<path.n;++i)last=std::max(last,path[i]);for(u32 i=begin;i<path.n;++i)time[path[i]]=last;}}
            }
            return time;
        }
    }
    struct Edge{u32 u,v,id;};Buffer<Edge>edges(0,m);
    PackedDigraph all(n,m);for(u32 i=0;i<m;++i){if(i+16<m)__builtin_prefetch(all.vertex.p+input[i+16][0],1);all.add(input[i][0],input[i][1]);}auto component=all.component_ids();all.vertex={};all.edge={};
    for(u32 i=0;i<m;++i)if(input[i][0]!=input[i][1]&&component[input[i][0]]==component[input[i][1]])edges.p[edges.n++]={input[i][0],input[i][1],i};
    component={};
    auto solve=[&](auto&&self,u32 l,u32 r,Buffer<Edge>e)->void{
        if(l>r||!e.n)return;u32 mid=(l+r)/2,count=0,used=0;Buffer<u32>id(0,std::min<usize>(n,2*e.n));
        for(auto x:std::span(e.p,e.n)){if(map[x.u]==~0u){map[x.u]=count++;id.p[id.n++]=x.u;}if(map[x.v]==~0u){map[x.v]=count++;id.p[id.n++]=x.v;}used+=x.id<=mid;}
        // Stable partitions preserve insertion order, so the midpoint prefix is
        // contiguous. Replace endpoints by compact IDs once, not on every pass.
        for(auto&x:std::span(e.p,e.n)){x.u=map[x.u];x.v=map[x.v];}for(u32 v:std::span(id.p,id.n))map[v]=~0u;
        PackedDigraph graph(count,used);for(u32 i=0;i<used;++i){if(i+16<used)__builtin_prefetch(graph.vertex.p+e[i+16].u,1);graph.add(e[i].u,e[i].v);}
        u32 components=0;graph.visit_components([&](u32 v){id[v]=components;},[&]{++components;});graph.vertex={};graph.edge={};
        Buffer<Edge>left(0,e.n),right(0,e.n);for(auto x:std::span(e.p,e.n)){if(x.id<=mid&&id[x.u]==id[x.v]){time[x.id]=mid;if(x.id<mid)left.p[left.n++]={x.u,x.v,x.id};}else if(id[x.u]==id[x.v])time[x.id]=x.id;else right.p[right.n++]={id[x.u],id[x.v],x.id};}
        e={};id={};if(l<mid)self(self,l,mid-1,std::move(left));if(mid<r)self(self,mid+1,r,std::move(right));
    };if(m)solve(solve,0,m-1,std::move(edges));return time;
}
template<u32 P=998244353>Buffer<u32> incremental_scc_sum(std::span<const u32>weight,std::span<const std::array<u32,2>>edges){
    u32 n=weight.size(),m=edges.size();auto time=scc_merge_times(n,edges);Buffer<u32>head(m),next(m),sum(n),answer(m);std::fill(head.p,head.p+m,~0u);memcpy(sum.p,weight.data(),n*4);
    for(u32 i=0;i<m;++i)if(time[i]<m){next[i]=head[time[i]];head[time[i]]=i;}DSU dsu(n);u32 total=0;
    for(u32 t=0;t<m;++t){for(u32 i=head[t];i!=~0u;i=next[i]){u32 a=dsu.leader(edges[i][0]),b=dsu.leader(edges[i][1]);if(a==b)continue;total=Mod<P>::add(total,Mod<P>::mul(sum[a],sum[b]));u32 value=Mod<P>::add(sum[a],sum[b]);dsu.merge(a,b);sum[dsu.leader(a)]=value;}answer[t]=total;}return answer;
}
}
