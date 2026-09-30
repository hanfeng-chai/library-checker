#pragma once
#include <toy/buffer.h>
namespace toy {
// Level k merges equal-position pairs between length-2^(Bits*k) intervals.
template<unsigned Bits=2> struct ParallelDSU {
    static_assert(Bits>=1&&Bits<=4);
    Buffer<i32> parent;
    array<usize,33> offset{};
    explicit ParallelDSU(u32 n){u32 levels=(bit_width(n)+Bits-1)/Bits;for(u32 k=0;k<levels;++k)offset[k+1]=offset[k]+n-(1u<<(Bits*k))+1;parent=Buffer<i32>(offset[levels]);fill(parent.p,parent.p+parent.n,-1);}
    static u32 leader(i32* p,u32 i){while(p[i]>=0){u32 j=p[i];if(p[j]<0)return j;p[i]=p[j];i=j;}return i;}
    template<class Merge> void link(u32 level,u32 first,u32 second,Merge& merge){
        i32* p=parent.p+offset[level];u32 a=leader(p,first),b=leader(p,second);if(a==b)return;if(p[a]>p[b])swap(a,b);p[a]+=p[b];p[b]=a;
        if(!level)merge(a,b);else{u32 part=1u<<(Bits*(level-1));for(u32 j=0;j<(1u<<Bits);++j)link(level-1,first+j*part,second+j*part,merge);}
    }
    template<class Merge> void unite(u32 a,u32 b,u32 length,Merge merge){
        if(!length||a==b)return;u32 k=(bit_width(length)-1)/Bits,block=1u<<(Bits*k);for(u32 rest=length;rest;){rest=rest>block?rest-block:0;link(k,a+rest,b+rest,merge);}
    }
    u32 leader(u32 i){return leader(parent.p,i);}
};
}
