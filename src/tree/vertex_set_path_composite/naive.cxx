#include <toy/affine_mont.h>
#include <toy/bidirectional.h>
#include <toy/heavy_light.h>
#include <toy/io.h>
using namespace toy;
int main(){using F=MontAffine<>;Reader in;Writer out;auto[n,q]=in.read_pair<6>();Buffer<F>a(n),ordered(n);for(auto& f:std::span(a.p,a.n)){auto[x,y]=in.read_pair<9>();f=F::encode(x,y);}HeavyLight h(n);for(u32 i=1;i<n;++i){auto[u,v]=in.read_pair<6>();h.add_edge(u,v);}h.build();for(u32 v=0;v<n;++v)ordered[h.position[v]]=a[v];ComposeMontAffine<> op;BidirectionalTree tree{std::span<const F>(ordered),F{},op};
    while(q--){u32 type=in.read<u32,1>();if(!type){u32 v=in.read<u32,6>();auto[x,y]=in.read_pair<9>();tree.set(h.position[v],F::encode(x,y));}
        else{auto[u,v]=in.read_pair<6>();u32 x=in.read<u32,9>();F left,right;while(h.head[u]!=h.head[v]){if(h.depth[h.head[u]]>h.depth[h.head[v]]){left=op(left,tree.fold<true>(h.position[h.head[u]],h.position[u]+1));u=h.parent[h.head[u]];}
                else{right=op(tree.fold(h.position[h.head[v]],h.position[v]+1),right);v=h.parent[h.head[v]];}}
            if(h.depth[u]>h.depth[v])left=op(left,tree.fold<true>(h.position[v],h.position[u]+1));else left=op(left,tree.fold(h.position[u],h.position[v]+1));out.write(op(left,right)(x));}}
}
