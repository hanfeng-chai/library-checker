#pragma once
#include <toy/graph.h>
#include <toy/radix_heap.h>
#include <toy/heap.h>
namespace toy {
struct ShortestPath {u64 distance;Buffer<u32> vertex;};
// If every vertex has at most one incoming edge, any source-target path is
// uniquely determined by walking backwards. nullopt means general Dijkstra.
inline std::optional<ShortestPath> single_predecessor_path(u32 n,std::span<const std::array<u32,3>>edges,u32 source,u32 target){
    if(edges.size()>n)return std::nullopt;
    // A repeated target in a short prefix already disproves the property.
    // This direct-mapped cache avoids touching an O(n) table on typical graphs.
    std::array<u32,1024>seen;seen.fill(~0u);for(u32 i=0;i<std::min<usize>(edges.size(),4096);++i){u32 v=edges[i][1];auto&old=seen[v&1023];if(old==v)return std::nullopt;old=v;}
    Buffer<WeightedArc>incoming(n);for(auto& e:std::span(incoming.p,incoming.n))e={~0u,0};
    for(auto e:edges){if(incoming[e[1]].to!=~0u)return std::nullopt;incoming[e[1]]={e[0],e[2]};}
    ShortestPath result{0,Buffer<u32>(0,n)};u32 v=target;
    while(v!=source){if(v==~0u||result.vertex.n==n)return ShortestPath{~0ull,{}};result.vertex.p[result.vertex.n++]=v;auto e=incoming[v];result.distance+=e.weight;v=e.to;}
    result.vertex.p[result.vertex.n++]=v;std::reverse(result.vertex.p,result.vertex.p+result.vertex.n);return result;
}
template<bool Radix=true,bool Bounded=false>ShortestPath shortest_path(const Graph<WeightedArc>& g,u32 source,u32 target){
    if constexpr(Radix){bool functional=true;for(u32 v=0;v<g.size();++v)if(g.offset[v+1]-g.offset[v]>1){functional=false;break;}
        if(functional){Buffer<WeightedArc>next(g.size());for(u32 v=0;v<g.size();++v)next[v]=g.offset[v]==g.offset[v+1]?WeightedArc{~0u,0}:g.edge[g.offset[v]];
            ShortestPath answer{0,Buffer<u32>(0,g.size())};u32 v=source;
            while(v!=target){if(v==~0u||answer.vertex.n==g.size())return {~0ull,{}};answer.vertex.p[answer.vertex.n++]=v;auto e=next[v];answer.distance+=e.weight;v=e.to;}
            answer.vertex.p[answer.vertex.n++]=v;return answer;
        }
    }
    struct Item{u64 distance;u32 vertex;bool operator<(Item b)const{return distance<b.distance;}};
    std::conditional_t<Radix,std::conditional_t<Bounded,BoundedRadixHeap,RadixHeap>,BinaryHeap<Item,true>> queue;
    if constexpr(!Radix)queue.values.reserve(g.edge.n+1);
    auto push=[&](u64 d,u32 v){if constexpr(Radix)queue.push(d,v);else queue.push({d,v});};
    u32 n=g.size();Buffer<u64> distance(n);Buffer<u32> parent(n);std::fill(distance.p,distance.p+n,~0ull);
    for(u32 v=0;v<n;++v)if(g.offset[v]==g.offset[v+1]&&v!=target)distance[v]=0;
    distance[source]=0;parent[source]=source;push(0,source);u64 bound=source==target?0:~0ull;
    while(!queue.empty()){
        auto[d,v]=queue.pop();if(d>=bound)break;if(d!=distance[v])continue;
        for(auto e:g[v]){u64 next=d+e.weight;if(next>=distance[e.to]||next>=bound)continue;
            distance[e.to]=next;parent[e.to]=v;
            if(e.to==target)bound=next;else push(next,e.to);
        }
    }
    ShortestPath result{distance[target],Buffer<u32>(n)};result.vertex.n=0;
    if(result.distance!=~0ull){for(u32 v=target;;v=parent[v]){result.vertex.p[result.vertex.n++]=v;if(v==source)break;}std::reverse(result.vertex.p,result.vertex.p+result.vertex.n);}
    return result;
}
}
