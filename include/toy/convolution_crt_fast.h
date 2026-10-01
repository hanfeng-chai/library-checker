#pragma once
#include <toy/convolution_crt.h>
#include <toy/convolution_u64.h>
namespace toy {
// Same exact three-prime reconstruction as convolution_crt, using fused NTTs.
template<u32 P> Buffer<u32> convolution_crt_fast(std::span<const u32>a,std::span<const u32>b){
    if(a.empty()||b.empty())return {};
    if(std::min(a.size(),b.size())<=16)return convolution_crt<P>(a,b);
    constexpr u32 p0=998244353,p1=1004535809,p2=469762049;
    constexpr u32 inv01=Mod<p1>::pow(p0,p1-2),inv012=Mod<p2>::pow(u64(p0)*p1%p2,p2-2);
    auto c0=u64_detail::under_prime<p0>(a,b),c1=u64_detail::under_prime<p1>(a,b),c2=u64_detail::under_prime<p2>(a,b);
    for(usize i=0;i<c0.n;++i){
        u32 x=Mod<p1>::mul(Mod<p1>::sub(c1[i],c0[i]),inv01);
        u32 y=Mod<p2>::mul(Mod<p2>::sub(c2[i],(c0[i]+u64(p0)*x)%p2),inv012);
        c0[i]=(c0[i]+u64(p0%P)*x+u64(u64(p0)*p1%P)*y)%P;
    }
    return c0;
}
}
