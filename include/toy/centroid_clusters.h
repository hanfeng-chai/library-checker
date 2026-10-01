#pragma once
#include <toy/centroid.h>
namespace toy {
template<class Visit,class Leaf> void centroid_clusters(const Adjacency& graph,Visit visit,Leaf leaf,u32 limit){
    u32 n=graph.size();Buffer<u32>parent(n),size(n),order(n),starts(n+1),tasks(n);Buffer<u8>removed(n);std::fill(removed.p,removed.p+n,0);tasks.n=0;if(n)tasks[tasks.n++]=0;Buffer<CentroidPoint>points(n);
    while(tasks.n){u32 root=tasks[--tasks.n],count=1;order[0]=root;parent[root]=~0u;
        for(u32 i=0;i<count;++i){u32 v=order[i];size[v]=1;for(u32 w:graph[v])if(!removed[w]&&w!=parent[v]){parent[w]=v;order[count++]=w;}}
        if(count<=limit){leaf(std::span<const u32>(order.p,count));for(u32 v:std::span(order.p,count))removed[v]=1;continue;}
        for(u32 i=count;i-->1;)size[parent[order[i]]]+=size[order[i]];
        u32 center=root;for(;;){u32 next=~0u;for(u32 w:graph[center])if(!removed[w]&&parent[w]==center&&size[w]>count/2){next=w;break;}if(next==~0u)break;center=next;}
        removed[center]=1;points.n=1;points[0]={center,0};starts.n=1;starts[0]=1;
        for(u32 child:graph[center])if(!removed[child]){u32 first=points.n;points[points.n++]={child,1};parent[child]=center;tasks[tasks.n++]=child;
            for(u32 i=first;i<points.n;++i){auto x=points[i];for(u32 w:graph[x.vertex])if(!removed[w]&&w!=parent[x.vertex]){parent[w]=x.vertex;points[points.n++]={w,x.distance+1};}}starts[starts.n++]=points.n;}
        visit(center,std::span<const CentroidPoint>(points),std::span<const u32>(starts));
    }
}
}
