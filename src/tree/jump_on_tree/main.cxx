#include <toy/tree_jump.h>
#include <toy/io.h>
using namespace toy;
int main(){Reader in;Writer out;auto[n,q]=in.read_pair<6>();TreeJump tree(n);for(u32 i=1;i<n;++i){auto[u,v]=in.read_pair<6>();tree.add_edge(u,v);}tree.build();std::array<u32,3>queries[128];i32 answers[128];
    while(q){u32 count=std::min(q,128u);for(u32 i=0;i<count;++i){auto[u,v]=in.read_pair<6>();queries[i]={u,v,in.read<u32,6>()};tree.prefetch(u,v);}for(u32 i=0;i<count;++i){auto[u,v,k]=queries[i];answers[i]=tree.jump(u,v,k);}out.write(std::span(answers,count));q-=count;}}
