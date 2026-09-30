#pragma once
#include <toy/mod.h>
namespace toy {
// c0*x*y+c1*x+c2*y+c3; coefficients are canonical residues modulo P.
template<u32 P=998244353> struct alignas(16) Bilinear {
    u32 c[4]{};
    Bilinear& operator+=(Bilinear b){auto x=_mm_add_epi32(_mm_load_si128((const __m128i*)c),_mm_load_si128((const __m128i*)b.c));_mm_store_si128((__m128i*)c,_mm_min_epu32(x,_mm_sub_epi32(x,_mm_set1_epi32(P))));return *this;}
    friend Bilinear operator-(Bilinear a,Bilinear b){auto x=_mm_sub_epi32(_mm_load_si128((const __m128i*)a.c),_mm_load_si128((const __m128i*)b.c));_mm_store_si128((__m128i*)a.c,_mm_min_epu32(x,_mm_add_epi32(x,_mm_set1_epi32(P))));return a;}
    u32 operator()(u32 x,u32 y)const{return (u64(Mod<P>::mul(c[0],x))*y+u64(c[1])*x+u64(c[2])*y+c[3])%P;}
};
}
