#include <toy/link_cut_sum.h>
#include <toy/packed_preorder.h>
#include <toy/io.h>
using namespace toy;
int main(){Reader in;Writer out;auto[n,q]=in.read_pair<6>();Buffer<i64>a(n);for(auto& x:std::span(a.p,a.n))x=in.read<u32,10>();Buffer<u32>position;
    auto tree=[&]{TreePreorder h(n);for(u32 i=1;i<n;++i){auto[u,v]=in.read_pair<6>();h.add_edge(u,v);}h.build();Buffer<i64>values(n);for(u32 v=0;v<n;++v)values[h.position[v]]=a[v];a={};LinkCutSum<i64,true>tree{std::span<const i64>(values)};tree.build_ordered_tree(std::span<const u32>(h.parent));position=std::move(h.position);return tree;}();
    while(q--){u32 type=in.read<u32,1>();if(!type){auto[u,v]=in.read_pair<6>();auto[w,x]=in.read_pair<6>();tree.cut(position[u],position[v]);tree.link(position[w],position[x]);}else if(type==1){u32 v=in.read<u32,6>();tree.add(position[v],in.read<u32,10>());}else{auto[u,v]=in.read_pair<6>();out.write(tree.side_sum(position[u],position[v]));}}}
