#include <toy/link_cut_affine.h>
#include <toy/io.h>
using namespace toy;
int main(){Reader in;Writer out;auto[n,q]=in.read_pair<6>();Buffer<Affine<>>a(n);for(auto& f:std::span(a.p,a.n)){auto[x,y]=in.read_pair<9>();f={x,y};}LinkCutAffine tree{std::span<const Affine<>>(a)};
    {Buffer<std::array<u32,2>>edges(n-1);for(auto& e:std::span(edges.p,edges.n)){auto[u,v]=in.read_pair<6>();e={u,v};}Adjacency graph(n,std::span<const std::array<u32,2>>(edges));tree.build_tree(graph);}
    while(q--){u32 type=in.read<u32,1>();if(!type){auto[u,v]=in.read_pair<6>();auto[w,x]=in.read_pair<6>();tree.cut(u,v);tree.link(w,x);}else if(type==1){u32 v=in.read<u32,6>();auto[a,b]=in.read_pair<9>();tree.set(v,{a,b});}else{auto[u,v]=in.read_pair<6>();out.write(tree.apply(u,v,in.read<u32,9>()));}}}
