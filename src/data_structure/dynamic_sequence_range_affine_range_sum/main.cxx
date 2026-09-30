#include <toy/io.h>
#include <toy/rope.h>
using namespace toy;
int main(){
    Reader in;Writer out;u32 n=in.read<u32,6>(),q=in.read<u32,6>();Buffer<u32> a(n);for(auto& x:span(a.p,a.n))x=in.read<u32,16>();if(!n&&q)while(*in.p<=' ')++in.p;
    Rope<998244353> sequence{span<const u32>(a),q};
    while(q--){u32 type=in.read<u32,1>(),l=in.read<u32,6>();
        if(type==0)sequence.insert(l,in.read<u32,16>());
        else if(type==1)sequence.erase(l);
        else{u32 r=in.read<u32,6>();if(type==2)sequence.reverse(l,r);else if(type==3){u32 a=in.read<u32,16>(),b=in.read<u32,16>();sequence.apply(l,r,{a,b});}else out.write(sequence.sum(l,r));}}
}
