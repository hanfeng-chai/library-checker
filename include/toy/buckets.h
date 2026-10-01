#pragma once
#include <toy/buffer.h>
namespace toy {
// Stable grouping by an integer key in [0,count). Offsets and items are contiguous.
template<class T> struct Buckets {
    Buffer<u32> offset;
    Buffer<T> data;
    template<class Key> Buckets(u32 count,Buffer<T> input,Key key):offset(count+1),data(input.n){
        std::fill(offset.p,offset.p+offset.n,0u);for(auto x:std::span(input.p,input.n))++offset[key(x)+1];
        for(u32 i=1;i<=count;++i)offset[i]+=offset[i-1];Buffer<u32> next(count);if(count)memcpy(next.p,offset.p,4*count);
        for(auto x:std::span(input.p,input.n))data[next[key(x)]++]=x;
    }
    std::span<const T> operator[](u32 key)const{return {data.p+offset[key],offset[key+1]-offset[key]};}
};
}
