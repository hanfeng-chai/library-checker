#include <toy/io.h>
#include <toy/beats.h>
using namespace toy;
int main(){Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>();Buffer<i64>a(n);for(auto& x:std::span(a.p,a.n))x=in.read<i64,16>();SegmentBeats tree{std::span<const i64>(a)};
    while(q--){u32 type=in.read<u32,1>();auto[l,r]=in.read_pair<6>();if(type==3)out.write(tree.sum(l,r));else{i64 x=in.read<i64,16>();if(!type)tree.chmin(l,r,x);else if(type==1)tree.chmax(l,r,x);else tree.add(l,r,x);}}}
