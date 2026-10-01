#include <toy/interval_decomposition.h>
#include <toy/io.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>();Buffer<u32>a(n);for(u32 i=0;i+1<n;i+=2){auto[x,y]=in.read_pair<6>();a[i]=x;a[i+1]=y;}if(n&1)a[n-1]=in.read<u32,6>();IntervalDecomposition tree{std::span<const u32>(a)};out.write(u32(tree.nodes.n-1));
    struct Item{u32 node,parent;};Buffer<Item>queue(tree.nodes.n-1);queue[0]={tree.root,~0u};for(u32 i=0,count=1;i<count;++i){auto item=queue[i];auto node=tree.nodes[item.node];out.write(i32(item.parent),' ');out.write(node.left,' ');out.write(node.right-1,' ');out.append(node.linear?"linear\n":"prime\n");for(u32 child=node.first;child;child=tree.nodes[child].next)queue[count++]={child,i};}}
