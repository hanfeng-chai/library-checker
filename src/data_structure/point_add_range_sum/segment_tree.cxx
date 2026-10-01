#include <toy/io.h>
#include <toy/segment.h>
using namespace toy;
int main(){Reader in;Writer out;auto[n,q]=in.read_pair<6>();Buffer<u64>a(n);for(auto& x:std::span(a.p,a.n))x=in.read<u64,10>();SegmentTree<u64,std::plus<u64>>tree{std::span<const u64>(a),0};a={};
    while(q--){u32 type=in.read<u32,1>(),x=in.read<u32,6>();if(!type)tree.set(x,tree.tree[tree.capacity+x]+in.read<u64,10>());else out.write(tree.fold(x,in.read<u32,6>()));}}
