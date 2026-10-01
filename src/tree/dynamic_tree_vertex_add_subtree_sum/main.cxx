#include <toy/offline_forest_sum.h>
#include <toy/io.h>
using namespace toy;
int main(){Reader in;Writer out;auto[n,q]=in.read_pair<6>();Buffer<i64>a(n);for(auto& x:std::span(a.p,a.n))x=in.read<i64,16>();OfflineForestSum<>tree(n,q);for(u32 i=1;i<n;++i){auto[u,v]=in.read_pair<6>();tree.link(u,v);}
    while(q--){u32 type=in.read<u32,1>();if(!type){auto[u,v]=in.read_pair<6>();auto[w,x]=in.read_pair<6>();tree.cut(u,v);tree.link(w,x);}else if(type==1){u32 v=in.read<u32,6>();tree.add_vertex(v,in.read<i64,16>());}else{auto[v,p]=in.read_pair<6>();tree.cut(v,p);tree.sum(v);tree.link(v,p);}}auto result=tree.solve(std::span<const i64>(a));out.write(std::span(result.p,result.n));}
