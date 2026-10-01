#pragma once
#include <toy/graph.h>
#include <toy/radix_heap.h>
namespace toy {
// Eppstein sidetracks: each shortest-tree suffix owns a persistent leftist heap
// of deviations. A monotone queue enumerates walks by their extra distance.
inline Buffer<i64> k_shortest_walk(u32 n,std::span<const std::array<u32,3>>edges,u32 source,u32 target,u32 k){
    Buffer<i64>answer(k);std::fill(answer.p,answer.p+k,-1);if(!k)return answer;
    if(edges.size()<=n){Buffer<WeightedArc>next(n);for(auto& e:std::span(next.p,next.n))e={~0u,0};bool functional=true;
        for(auto e:edges){if(next[e[0]].to!=~0u){functional=false;break;}next[e[0]]={e[1],e[2]};}
        if(functional){Buffer<u64>at(n);std::fill(at.p,at.p+n,~0ull);u32 v=source,step=0,target_step=~0u;u64 distance=0,first=~0ull;
            while(v!=~0u&&at[v]==~0ull){at[v]=distance;if(v==target){first=distance;target_step=step;}auto e=next[v];next[v].weight=step++;distance+=e.weight;v=e.to;}
            if(first==~0ull)return answer;answer[0]=first;
            if(v!=~0u&&next[v].weight<=target_step){u64 period=distance-at[v];for(u32 i=1;i<k;++i)answer[i]=first+u64(i)*period;}
            return answer;
        }
    }
    Buffer<std::array<u32,2>>reverse(edges.size());for(u32 i=0;i<edges.size();++i)reverse[i]={edges[i][1],edges[i][0]};auto back=indexed_graph(n,std::span<const std::array<u32,2>>(reverse),true);reverse={};
    Buffer<u64>distance(n);Buffer<u32>parent(n),order(0,n);std::fill(distance.p,distance.p+n,~0ull);std::fill(parent.p,parent.p+n,~0u);RadixHeap queue;distance[target]=0;queue.push(0,target);
    while(!queue.empty()){auto[d,v]=queue.pop();if(d!=distance[v])continue;order.p[order.n++]=v;for(auto e:back[v]){u64 next=d+edges[e.id][2];if(next<distance[e.to]){distance[e.to]=next;parent[e.to]=e.id;queue.push(next,e.to);}}}
    if(distance[source]==~0ull)return answer;answer[0]=distance[source];
    struct Node{u64 cost;u32 to,left=0,right=0,height=1;};Buffer<Node>heap(1,edges.size()+8*n+1);heap[0]={0,0,0,0,0};Buffer<u32>root(n);std::fill(root.p,root.p+n,0u);
    auto append=[&](Node x){if(heap.n==heap.capacity)heap.reserve(heap.capacity*2);u32 id=heap.n++;heap[id]=x;return id;};
    auto meld=[&](auto&&self,u32 a,u32 b,bool persistent)->u32{if(!a||!b)return a|b;if(heap[a].cost>heap[b].cost)std::swap(a,b);if(persistent)a=append(heap[a]);u32 right=self(self,heap[a].right,b,persistent);heap[a].right=right;if(heap[heap[a].left].height<heap[right].height)std::swap(heap[a].left,heap[a].right);heap[a].height=heap[heap[a].right].height+1;return a;};
    Buffer<u32>offset(n+1);std::fill(offset.p,offset.p+n+1,0u);u64 maximum_delta=0;
    for(u32 i=0;i<edges.size();++i)if(i!=parent[edges[i][0]]&&distance[edges[i][1]]!=~0ull)++offset[edges[i][0]+1];
    for(u32 v=1;v<=n;++v)offset[v]+=offset[v-1];Buffer<u32>cursor(n);memcpy(cursor.p,offset.p,n*4);heap.n=offset[n]+1;
    for(u32 i=0;i<edges.size();++i){auto e=edges[i];if(i==parent[e[0]]||distance[e[1]]==~0ull)continue;u64 delta=u64(e[2])+distance[e[1]]-distance[e[0]];maximum_delta=std::max(maximum_delta,delta);heap[1+cursor[e[0]]++]={delta,e[1]};}
    // Linear-time local heap construction on contiguous rows. A complete
    // binary min-heap also satisfies the leftist rank condition.
    for(u32 v=0;v<n;++v){u32 start=offset[v]+1,length=offset[v+1]-offset[v];if(!length)continue;root[v]=start;std::make_heap(heap.p+start,heap.p+start+length,[](Node a,Node b){return a.cost>b.cost;});
        for(u32 i=length;i--;){auto&x=heap[start+i];x.left=2*i+1<length?start+2*i+1:0;x.right=2*i+2<length?start+2*i+2:0;x.height=heap[x.right].height+1;}
    }
    for(u32 v:std::span(order.p,order.n))if(v!=target)root[v]=meld(meld,root[v],root[edges[parent[v]][1]],true);
    queue={};
    auto enumerate=[&]<class Heap>(){Heap candidates;candidates.last=distance[source];if(root[source])candidates.push(distance[source]+heap[root[source]].cost,root[source]);
        for(u32 i=1;i<k&&!candidates.empty();++i){auto[d,id]=candidates.pop();answer[i]=d;auto node=heap[id];for(u32 child:{node.left,node.right})if(child)candidates.push(d-node.cost+heap[child].cost,child);u32 next=root[node.to];if(next)candidates.push(d+heap[next].cost,next);}
    };
    // Every successor increases the key by at most the largest sidetrack.
    if(maximum_delta<(1ull<<31))enumerate.template operator()<BoundedRadixHeap>();else enumerate.template operator()<RadixHeap>();
    return answer;
}
}
