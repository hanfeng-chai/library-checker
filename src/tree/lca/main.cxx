#include <toy/io.h>
#include <toy/ordered_lca.h>
using namespace toy;
int main(){Reader in;Writer out;auto[n,q]=in.read_pair<6>();Buffer<u32>p(n),a(q),b(q);p[0]=0;for(u32 i=1;i<n;++i)p[i]=in.read<u32,6>();OrderedLCA tree{std::span<const u32>(p)};if(n==1&&q)while(*in.p<=' ')++in.p;
    for(u32 i=0;i<q;++i){auto[x,y]=in.read_pair<6>();a[i]=x;b[i]=y;}
    for(u32 i=0;i<q;++i){if(i+8<q){__builtin_prefetch(tree.label.p+a[i+8],0,3);__builtin_prefetch(tree.label.p+b[i+8],0,3);}a[i]=tree.lca(a[i],b[i]);}
    out.write(std::span(a.p,a.n));}
