#pragma once
#include <toy/buffer.h>

namespace toy {
// Four byte histograms are counted in one pass, then reused by stable scatters.
template<class T,class Key>void radix_sort32(std::span<T> values,Key key){
    if(values.empty())return;std::array<std::array<u32,256>,4>count{};
    for(auto x:values){u32 k=key(x);for(u32 d=0;d<4;++d)++count[d][(k>>(8*d))&255];}
    Buffer<T>temporary(values.size());T*from=values.data(),*to=temporary.p;
    for(u32 d=0;d<4;++d){auto&row=count[d];if(row[(u32(key(from[0]))>>(8*d))&255]==values.size())continue;
        u32 sum=0;for(auto&x:row){u32 n=x;x=sum;sum+=n;}
        for(usize i=0;i<values.size();++i){u32 b=(u32(key(from[i]))>>(8*d))&255;to[row[b]++]=from[i];}std::swap(from,to);
    }
    if(from!=values.data())memcpy(values.data(),from,values.size_bytes());
}
// Stable LSD sort of trivial records by an unsigned key smaller than 2^Bits.
// Small records use insertion sort; a constant digit needs no scatter pass.
template<unsigned Bits = 32, unsigned Digit = 8, class T, class Key>
void radix_sort(std::span<T> values, Key key) {
    static_assert(Bits >= 1 && Bits <= 64 && Digit >= 1 && Digit <= 16);
    if (values.size() < 64) {
        for (usize i = 1; i < values.size(); ++i) {
            T x = values[i]; auto k = key(x); usize j = i;
            while (j && key(values[j - 1]) > k) { values[j] = values[j - 1]; --j; }
            values[j] = x;
        }
        return;
    }
    constexpr usize buckets = usize(1) << Digit, mask = buckets - 1;
    Buffer<T> temporary(values.size()); T* from = values.data(); T* to = temporary.p;
    for (unsigned shift = 0; shift < Bits; shift += Digit) {
        std::array<u32, buckets> count{};
        for (usize i = 0; i < values.size(); ++i) ++count[(u64(key(from[i])) >> shift) & mask];
        if (count[(u64(key(from[0])) >> shift) & mask] == values.size()) continue;
        u32 sum = 0; for (auto& c : count) { u32 length = c; c = sum; sum += length; }
        for (usize i = 0; i < values.size(); ++i) to[count[(u64(key(from[i])) >> shift) & mask]++] = from[i];
        std::swap(from, to);
    }
    if (from != values.data()) memcpy(values.data(), from, values.size_bytes());
}
}
