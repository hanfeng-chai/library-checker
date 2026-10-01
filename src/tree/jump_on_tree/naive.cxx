#include <toy/heavy_light.h>
#include <toy/io.h>
using namespace toy;
int main(){Reader in;Writer out;auto[n,q]=in.read_pair<6>();HeavyLight tree(n);for(u32 i=1;i<n;++i){auto[a,b]=in.read_pair<6>();tree.add_edge(a,b);}tree.build();
    while(q--){auto[a,b]=in.read_pair<6>();u32 k=in.read<u32,6>();out.write(i32(tree.jump(a,b,k)));}}
