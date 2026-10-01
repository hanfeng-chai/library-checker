#include <toy/centroid_sum.h>
#include <toy/io.h>
using namespace toy;
int main(){Reader in;Writer out;auto[n,q]=in.read_pair<6>();Buffer<i64>a(n);for(auto& x:std::span(a.p,a.n))x=in.read<i64,10>();Buffer<std::array<u32,2>>edges(n-1);for(auto& e:std::span(edges.p,edges.n)){auto[u,v]=in.read_pair<6>();e={u,v};}Adjacency graph(n,std::span<const std::array<u32,2>>(edges));CentroidSum<i64> tree(graph,std::span<const i64>(a));
    while(q--){auto[type,v]=in.read_pair<6>();if(!type)tree.add(v,in.read<i64,10>());else{auto[l,r]=in.read_pair<6>();out.write(tree.sum(v,l,r));}}}
