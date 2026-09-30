#pragma once
#include <toy/buffer.h>

namespace toy {
namespace convolution_detail {
// Radix-4 and four parallel 8-point leaves, adapted from submission #393372.
// Moduli are primes below 2^30. Butterfly values stay below 4*mod;
// each leaf accumulates eight canonical products in 64-bit lanes.
using Vec = __m256i;
[[gnu::always_inline]] inline auto store(void*p,Vec x){
    _mm256_store_si256((Vec*)p,x);
}
[[gnu::always_inline]] inline auto load256(const void*p){
    return _mm256_load_si256((const Vec*)p);
}
[[gnu::always_inline]] inline auto loadu256(const void*p){
    return _mm256_loadu_si256((const __m256i_u*)p);
}
[[gnu::always_inline]] constexpr auto shrk(u32 x,u32 M){
    return std::min(x,x-M);
}
[[gnu::always_inline]] constexpr auto reduce(u64 x,u32 ninv,u32 M)->u32 {
    return (x+u64(u32(x)*ninv)*M)>>32;
}
[[gnu::always_inline]] constexpr auto mul(u32 x,u32 y,u32 ninv,u32 M){
    return reduce(u64(x)*y,ninv,M);
}
[[gnu::always_inline]] constexpr auto mul_b_fixed(u32 x,u32 y,u32 binv,u32 M)->u32 {
    return (u64(x)*y+u64(binv*x)*M)>>32;
}
[[gnu::always_inline]] constexpr auto mul_s(u32 x,u32 y,u32 ninv,u32 M){
    return shrk(reduce(u64(x)*y,ninv,M),M);
}
[[gnu::always_inline]] constexpr auto qpw(u32 a,u32 b,u32 ninv,u32 M,u32 r){
    for(;b;b>>=1,a=mul(a,a,ninv,M)){
        if(b&1){
            r=mul(r,a,ninv,M);
        }
    }
    return r;
}
[[gnu::always_inline]] constexpr auto qpw_s(u32 a,u32 b,u32 ninv,u32 M,u32 r){
    return shrk(qpw(a,b,ninv,M,r),M);
}
[[gnu::always_inline]] inline auto shrk32(Vec x,Vec M){
    return _mm256_min_epu32(x,_mm256_sub_epi32(x,M));
}
[[gnu::always_inline]] inline auto dilt32(Vec x,Vec M){
    return _mm256_min_epu32(x,_mm256_add_epi32(x,M));
}
[[gnu::always_inline]] inline auto add32(Vec x,Vec y){
    return _mm256_add_epi32(x,y);
}
[[gnu::always_inline]] inline auto Lsub32(Vec x,Vec y,Vec M){
    return _mm256_add_epi32(x,_mm256_sub_epi32(M,y));
}
[[gnu::always_inline]] inline auto add32(Vec x,Vec y,Vec M){
    return shrk32(_mm256_add_epi32(x,y),M);
}
[[gnu::always_inline]] inline auto sub32(Vec x,Vec y,Vec M){
    return dilt32(_mm256_sub_epi32(x,y),M);
}
[[gnu::always_inline]] inline auto lmove(Vec x){
    return _mm256_bsrli_epi128(x,4);
}
[[gnu::always_inline]] inline auto reduce(Vec a,Vec b,Vec ninv,Vec M){
    auto c=_mm256_mul_epu32(a,ninv),d=_mm256_mul_epu32(b,ninv);
    c=_mm256_mul_epu32(c,M),d=_mm256_mul_epu32(d,M);
    return _mm256_or_si256(lmove(_mm256_add_epi64(a,c)),_mm256_add_epi64(b,d));
}
template<int b_only_even=0>[[gnu::always_inline]] inline auto mul(Vec a,Vec b,Vec ninv,Vec M){
    return reduce(_mm256_mul_epu32(a,b),_mm256_mul_epu32(lmove(a),b_only_even?b:lmove(b)),ninv,M);
}
template<int b_only_even=0>[[gnu::always_inline]] inline auto mul_b_fixed(Vec a,Vec b,Vec bninv,Vec M){
    Vec cc=_mm256_mul_epu32(a,bninv),dd=_mm256_mul_epu32(lmove(a),b_only_even?bninv:lmove(bninv));
    Vec c=_mm256_mul_epu32(a,b),d=_mm256_mul_epu32(lmove(a),b_only_even?b:lmove(b));
    cc=_mm256_mul_epu32(cc,M),dd=_mm256_mul_epu32(dd,M);
    return _mm256_or_si256(lmove(_mm256_add_epi64(c,cc)),_mm256_add_epi64(d,dd));
}
[[gnu::always_inline]] inline auto mul_upd_rt(Vec a,Vec bu,Vec M){
    auto cc=_mm256_mul_epu32(a,bu),c=_mm256_mul_epu32(a,_mm256_srli_epi64(bu,32));
    cc=_mm256_mul_epu32(cc,M);
    return shrk32(_mm256_srli_epi64(_mm256_add_epi64(c,cc),32),M);
}
[[gnu::always_inline]] inline auto expand(u32 x){
    return _mm256_set1_epi32(x);
}

inline constexpr int max_log=32,cache_log=6;
inline constexpr usize cache_size=usize(1)<<cache_log;
static_assert(cache_log%2==0);
// Four independent products modulo x^8-w. Each vector holds eight adjacent
// coefficients; wrapped terms are multiplied by w. Canonical factors let each
// u64 accumulator hold eight products before a single Montgomery reduction.
[[gnu::always_inline]] inline auto leaf4(u32*__restrict__ f,u32*__restrict__ g,std::array<u32,4> ww,Vec Ninv,Vec Mod,Vec Mod2){
    alignas(64) u32 awa[4][16];
    alignas(64) Vec res0[4]={},res1[4]={};
    #pragma GCC unroll 4
    for(auto i=0;i<4;++i){
        store(g+i*8,shrk32(shrk32(load256(g+i*8),Mod2),Mod));
        auto ff=load256(f+i*8);
        ff=shrk32(ff,Mod2);
        const auto ffw=shrk32(mul<1>(ff,expand(ww[i]),Ninv,Mod),Mod);
        ff=shrk32(ff,Mod);
        store(awa[i],ffw),store(awa[i]+8,ff);
    }
    for(auto i=0;i<8;++i){
        #pragma GCC unroll 4
        for(auto j=0;j<4;++j){
            const auto bi=expand(g[j*8+i]);
            const auto aj=loadu256(awa[j]+8-i);
            const auto aj2=lmove(aj);
            res0[j]=_mm256_add_epi64(res0[j],_mm256_mul_epu32(bi,aj));
            res1[j]=_mm256_add_epi64(res1[j],_mm256_mul_epu32(bi,aj2));
        }
    }
    #pragma GCC unroll 4
    for(auto i=0;i<4;++i){
        store(f+i*8,shrk32(reduce(res0[i],res1[i],Ninv,Mod),Mod2));
    }
}
struct NTTInfo{
    u32 mod,mod2,ninv,one,r2,r3,imag,imagninv,RT3[max_log];
    alignas(32) array<u32,8> rt3[max_log-2],rt3i[max_log-2],bwb,bwbi;
    [[gnu::always_inline]] constexpr NTTInfo(u32 m):
    mod(m),mod2(m*2),ninv([&]{auto n=2+m;for(auto i=0;i<4;++i){n*=2+m*n;}return n;}()),
    one((-m)%m),r2((-u64(m))%m),r3(mul_s(r2,r2,ninv,m)),
    imag{},imagninv{},RT3{},rt3{},rt3i{},bwb{},bwbi{}{
        auto k=__builtin_ctz(m-1);
        auto _g=mul(3,r2,ninv,mod);
        // Only -1 certifies a nonzero quadratic nonresidue; reject zero too.
        for(;;++_g){
            if(qpw_s(_g,mod>>1,ninv,mod,one)==mod-one){
                break;
            }
        }
        _g=qpw(_g,mod>>k,ninv,mod,one);
        u32 rt1[max_log-1]={},rt1i[max_log-1]={};
        rt1[k-2]=_g,rt1i[k-2]=qpw(_g,mod-2,ninv,mod,one);
        for(auto i=k-2;i>0;--i){
            rt1[i-1]=mul(rt1[i],rt1[i],ninv,mod);
            rt1i[i-1]=mul(rt1i[i],rt1i[i],ninv,mod);
        }
        imag=rt1[0],imagninv=imag*ninv;
        bwb={rt1[1],0,rt1[0],0,mod-mul_s(rt1[0],rt1[1],ninv,mod)};
        bwbi={rt1i[1],0,rt1i[0],0,mul_s(rt1i[0],rt1i[1],ninv,mod)};
        auto pr=one,pri=one;
        for(auto i=0;i<k-2;++i){
            const auto r=mul_s(pr,rt1[i+1],ninv,mod),ri=mul_s(pri,rt1i[i+1],ninv,mod);
            const auto r2=mul_s(r,r,ninv,mod),r2i=mul_s(ri,ri,ninv,mod);
            const auto r3=mul_s(r,r2,ninv,mod),r3i=mul_s(ri,r2i,ninv,mod);
            rt3[i]={r*ninv,r,r2*ninv,r2,r3*ninv,r3};
            RT3[i+2]=rt3[i][1];
            rt3i[i]={ri*ninv,ri,r2i*ninv,r2i,r3i*ninv,r3i};
            pr=mul(pr,rt1i[i+1],ninv,mod),pri=mul(pri,rt1[i+1],ninv,mod);
        }
    }
    // n counts 8-coefficient vectors. Radix-4 DIF stops at polynomial leaves;
    // finish the small levels per cache chunk instead of rescanning all memory.
    [[gnu::always_inline]] inline auto forward(Vec*const f,usize n)const{
        alignas(32) array<u32,8> st_1[max_log>>1];
        const auto Mod=expand(mod),Mod2=expand(mod2),Ninv=expand(ninv),Imag=expand(imag),ImagNinv=expand(imagninv);
        const auto id24=_mm256_set_epi32(4,0,2,0,4,0,2,0);
        const auto lgn=__builtin_ctzll(n);
        std::fill(st_1,st_1+(lgn>>1),bwb);
        const auto nn=n>>(lgn&1),m=std::min(n,cache_size);
        if(nn!=n){
            for(usize i=0;i<nn;++i){
                auto p0=f+i,p1=f+nn+i;
                const auto f0=load256(p0),f1=load256(p1);
                const auto g0=add32(f0,f1,Mod2),g1=Lsub32(f0,f1,Mod2);
                store(p0,g0),store(p1,g1);
            }
        }
        for(auto L=nn>>2;L>0;L>>=2){
            for(usize i=0;i<L;++i){
                auto p0=f+i,p1=p0+L,p2=p1+L,p3=p2+L;
                const auto f1=load256(p1),f3=load256(p3),f2=load256(p2),f0=load256(p0);
                const auto g3=mul_b_fixed<1>(Lsub32(f1,f3,Mod2),Imag,ImagNinv,Mod),g1=add32(f1,f3,Mod2);
                const auto g0=add32(f0,f2,Mod2),g2=sub32(f0,f2,Mod2);
                const auto h0=add32(g0,g1,Mod2),h1=Lsub32(g0,g1,Mod2);
                const auto h2=add32(g2,g3),h3=Lsub32(g2,g3,Mod2);
                store(p0,h0),store(p1,h1),store(p2,h2),store(p3,h3);
            }
        }
        int t=std::min(cache_log,lgn)&-2,p=(t-2)>>1;
        for(usize j=0;j<n;j+=m,t=__builtin_ctzll(j)&-2,p=(t-2)>>1){
            auto const g=f+j;
            for(usize l=(usize(1)<<t),L=l>>2;L>1;l=L,L>>=2,t-=2,--p){
                auto rt=load256(st_1+p);
                for(usize i=(j==0?l:0),k=(j+i)>>t;i<m;i+=l,++k){
                    const auto r1=_mm256_permutevar8x32_epi32(rt,id24);
                    const auto r1Ninv=_mm256_permutevar8x32_epi32(_mm256_mul_epu32(rt,Ninv),id24);
                    rt=mul_upd_rt(rt,load256(rt3+__builtin_ctzll(~k)),Mod);
                    const auto r2=_mm256_shuffle_epi32(r1,_MM_SHUFFLE(1,1,1,1)),nr3=_mm256_shuffle_epi32(r1,_MM_SHUFFLE(3,3,3,3));
                    const auto r2Ninv=_mm256_shuffle_epi32(r1Ninv,_MM_SHUFFLE(1,1,1,1)),nr3Ninv=_mm256_shuffle_epi32(r1Ninv,_MM_SHUFFLE(3,3,3,3));
                    for(usize j=0;j<L;++j){
                        auto p0=g+i+j,p1=p0+L,p2=p1+L,p3=p2+L;
                        const auto f1=load256(p1),f3=load256(p3),f2=load256(p2),f0=load256(p0);
                        const auto g1=mul_b_fixed<1>(f1,r1,r1Ninv,Mod),ng3=mul_b_fixed<1>(f3,nr3,nr3Ninv,Mod);
                        const auto g2=mul_b_fixed<1>(f2,r2,r2Ninv,Mod),g0=shrk32(f0,Mod2);
                        const auto h3=mul_b_fixed<1>(add32(g1,ng3),Imag,ImagNinv,Mod),h1=sub32(g1,ng3,Mod2);
                        const auto h0=add32(g0,g2,Mod2),h2=sub32(g0,g2,Mod2);
                        const auto o0=add32(h0,h1),o1=Lsub32(h0,h1,Mod2);
                        const auto o2=add32(h2,h3),o3=Lsub32(h2,h3,Mod2);
                        store(p0,o0),store(p1,o1),store(p2,o2),store(p3,o3);
                    }
                }
                store(st_1+p,rt);
            }
            {//L == 1
                auto rt=load256(st_1);
                for(usize i=j+(j==0)*4;i<j+m;i+=4){
                    const auto r1=_mm256_permutevar8x32_epi32(rt,id24);
                    rt=mul_upd_rt(rt,load256(rt3+__builtin_ctzll(~i>>2)),Mod);
                    auto p0=f+i,p1=p0+1,p2=p0+2,p3=p0+3;
                    const auto f1=load256(p1),f3=load256(p3),f2=load256(p2),f0=load256(p0);
                    const auto r2=_mm256_shuffle_epi32(r1,_MM_SHUFFLE(1,1,1,1)),nr3=_mm256_shuffle_epi32(r1,_MM_SHUFFLE(3,3,3,3));
                    const auto g1=mul<1>(f1,r1,Ninv,Mod),ng3=mul<1>(f3,nr3,Ninv,Mod);
                    const auto g2=mul<1>(f2,r2,Ninv,Mod),g0=shrk32(f0,Mod2);
                    const auto h3=mul_b_fixed<1>(add32(g1,ng3),Imag,ImagNinv,Mod),h1=sub32(g1,ng3,Mod2);
                    const auto h0=add32(g0,g2,Mod2),h2=sub32(g0,g2,Mod2);
                    const auto o0=add32(h0,h1),o1=Lsub32(h0,h1,Mod2);
                    const auto o2=add32(h2,h3),o3=Lsub32(h2,h3,Mod2);
                    store(p0,o0),store(p1,o1),store(p2,o2),store(p3,o3);
                }
                store(st_1,rt);
            }
        }
    }
    // Leaf products carry R^-1. The first inverse stage multiplies by R^2/n,
    // combining normalization and representation conversion in one reduction.
    [[gnu::always_inline]] inline auto inverse(Vec*const f,usize n)const{
        alignas(32) array<u32,8> st_1[max_log>>1];
        const auto Mod=expand(mod),Mod2=expand(mod2),Ninv=expand(ninv),Imag=expand(imag),ImagNinv=expand(imagninv);
        const auto id24=_mm256_set_epi32(4,0,2,0,4,0,2,0);
        const auto lgn=__builtin_ctzll(n);
        std::fill(st_1,st_1+(lgn>>1),bwbi);
        const auto nn=n>>(lgn&1),m=std::min(n,cache_size);
        const auto fx=mul_s(mod-((mod-1)>>lgn),r3,ninv,mod);
        const auto Fx=expand(fx),FxNinv=expand(fx*ninv);
        store(st_1,Fx);
        for(usize j=0;j<n;j+=m){
            auto tt=__builtin_ctzll(j+m),t=4,p=1;
            {//L == 1, append coefficient
                auto rt=load256(st_1);
                for(usize i=j;i<j+m;i+=4){
                    const auto r1=_mm256_permutevar8x32_epi32(rt,id24);
                    rt=mul_upd_rt(rt,load256(rt3i+__builtin_ctzll(~i>>2)),Mod);
                    auto const p0=f+i,p1=p0+1,p2=p1+1,p3=p2+1;
                    const auto f2=load256(p2),f3=load256(p3),f0=load256(p0),f1=load256(p1);
                    const auto r2=_mm256_shuffle_epi32(r1,_MM_SHUFFLE(1,1,1,1)),r3=_mm256_shuffle_epi32(r1,_MM_SHUFFLE(3,3,3,3));
                    const auto g3=mul_b_fixed<1>(Lsub32(f3,f2,Mod2),Imag,ImagNinv,Mod),g2=add32(f2,f3,Mod2);
                    const auto g0=add32(f0,f1,Mod2),g1=sub32(f0,f1,Mod2);
                    const auto h2=Lsub32(g0,g2,Mod2),h3=Lsub32(g1,g3,Mod2);
                    const auto h0=add32(g0,g2),h1=add32(g1,g3);
                    const auto o2=mul<1>(h2,r2,Ninv,Mod),o0=mul_b_fixed<1>(h0,Fx,FxNinv,Mod);
                    const auto o1=mul<1>(h1,r1,Ninv,Mod),o3=mul<1>(h3,r3,Ninv,Mod);
                    store(p0,o0),store(p1,o1),store(p2,o2),store(p3,o3);
                }
                store(st_1,rt);
            }
            for(usize l=16,L=4;t<=tt;L=l,l<<=2,t+=2,++p){
                usize diff=j+m-std::max(l,m),i=0;
                Vec rt=load256(st_1+p),*const g=f+diff;
                if(diff==0){
                    if(l==n){
                        for(usize i=0;i<L;++i){
                            auto const p0=f+i,p1=p0+L,p2=p1+L,p3=p2+L;
                            const auto f2=load256(p2),f3=load256(p3),f0=load256(p0),f1=load256(p1);
                            const auto g3=mul_b_fixed<1>(Lsub32(f3,f2,Mod2),Imag,ImagNinv,Mod),g2=add32(f2,f3,Mod2);
                            const auto g0=add32(f0,f1,Mod2),g1=sub32(f0,f1,Mod2);
                            const auto h0=add32(g0,g2,Mod2),h1=add32(g1,g3,Mod2);
                            const auto h2=sub32(g0,g2,Mod2),h3=sub32(g1,g3,Mod2);
                            const auto o0=shrk32(h0,Mod),o1=shrk32(h1,Mod);
                            const auto o2=shrk32(h2,Mod),o3=shrk32(h3,Mod);
                            store(p0,o0),store(p1,o1),store(p2,o2),store(p3,o3);
                        }
                    }
                    else{
                        for(usize i=0;i<L;++i){
                            auto const p0=f+i,p1=p0+L,p2=p1+L,p3=p2+L;
                            const auto f2=load256(p2),f3=load256(p3),f0=load256(p0),f1=load256(p1);
                            const auto g3=mul_b_fixed<1>(Lsub32(f3,f2,Mod2),Imag,ImagNinv,Mod),g2=add32(f2,f3,Mod2);
                            const auto g0=add32(f0,f1,Mod2),g1=sub32(f0,f1,Mod2);
                            const auto h0=add32(g0,g2,Mod2),h1=add32(g1,g3,Mod2);
                            const auto h2=sub32(g0,g2,Mod2),h3=sub32(g1,g3,Mod2);
                            store(p0,h0),store(p1,h1),store(p2,h2),store(p3,h3);
                        }
                    }
                    i=l;
                }
                for(usize k=(j+i)>>t;i<m;i+=l,++k){
                    const auto r1=_mm256_permutevar8x32_epi32(rt,id24);
                    const auto r1Ninv=_mm256_permutevar8x32_epi32(_mm256_mul_epu32(rt,Ninv),id24);
                    rt=mul_upd_rt(rt,load256(rt3i+__builtin_ctzll(~k)),Mod);
                    const auto r2=_mm256_shuffle_epi32(r1,_MM_SHUFFLE(1,1,1,1)),r3=_mm256_shuffle_epi32(r1,_MM_SHUFFLE(3,3,3,3));
                    const auto r2Ninv=_mm256_shuffle_epi32(r1Ninv,_MM_SHUFFLE(1,1,1,1)),r3Ninv=_mm256_shuffle_epi32(r1Ninv,_MM_SHUFFLE(3,3,3,3));
                    for(usize j=0;j<L;++j){
                        auto const p0=g+i+j,p1=p0+L,p2=p1+L,p3=p2+L;
                        const auto f2=load256(p2),f3=load256(p3),f0=load256(p0),f1=load256(p1);
                        const auto g3=mul_b_fixed<1>(Lsub32(f3,f2,Mod2),Imag,ImagNinv,Mod),g2=add32(f2,f3,Mod2);
                        const auto g0=add32(f0,f1,Mod2),g1=sub32(f0,f1,Mod2);
                        const auto h2=Lsub32(g0,g2,Mod2),h3=Lsub32(g1,g3,Mod2);
                        const auto h0=add32(g0,g2),h1=add32(g1,g3);
                        const auto o2=mul_b_fixed<1>(h2,r2,r2Ninv,Mod),o0=shrk32(h0,Mod2);
                        const auto o1=mul_b_fixed<1>(h1,r1,r1Ninv,Mod),o3=mul_b_fixed<1>(h3,r3,r3Ninv,Mod);
                        store(p0,o0),store(p1,o1),store(p2,o2),store(p3,o3);
                    }
                }
                store(st_1+p,rt);
            }
        }
        if(nn!=n){
            for(usize i=0;i<nn;++i){
                auto const p0=f+i,p1=f+nn+i;
                const auto f0=load256(p0),f1=load256(p1);
                const auto g0=add32(f0,f1,Mod2),g1=sub32(f0,f1,Mod2);
                const auto h0=shrk32(g0,Mod),h1=shrk32(g1,Mod);
                store(p0,h0),store(p1,h1);
            }
        }
    }
    [[gnu::always_inline]] inline auto products(Vec*__restrict__ f,Vec*__restrict__ g,usize lm)const{
        auto RR=one;
        const auto Ninv=expand(ninv),Mod=expand(mod),Mod2=expand(mod2);
        for(usize i=0;i<lm;i+=4){
            const auto RRi=mul_b_fixed(RR,imag,imagninv,mod);
            leaf4((u32*)(f+i),(u32*)(g+i),{RR,mod2-RR,RRi,mod2-RRi},Ninv,Mod,Mod2);
            RR=mul(RR,RT3[__builtin_ctzll(i+4)],ninv,mod);
        }
    }
};


template<u32 Mod>
inline constexpr NTTInfo info{Mod};
}

// Consumes both arrays. Coefficients are ordinary residues in [0, Mod).
// The partial transform needs N/8 roots; its leaves multiply modulo x^8-w.
template<u32 Mod = 998244353>
Buffer<u32> convolution(Buffer<u32> a, Buffer<u32> b) {
    static_assert(Mod > 2 && Mod < (1u << 30) && (Mod - 1) % 8 == 0);
    if (!a.n || !b.n) return {};
    if (a.n < b.n) swap(a, b);
    usize n = a.n, m = b.n, count = n + m - 1;
    if (m <= 16) {
        a.resize(count);
        for (usize k = count; k--;) {
            u64 sum = 0;
            for (usize j = k < n ? 0 : k - n + 1; j < min(m, k + 1); ++j)
                sum += u64(a[k - j]) * b[j];
            a[k] = sum % Mod;
        }
        return a;
    }
    usize size = max<usize>(64, bit_ceil(count));
    a.resize(size); b.resize(size);
    const auto& ntt = convolution_detail::info<Mod>;
    auto* x = (convolution_detail::Vec*)a.p;
    auto* y = (convolution_detail::Vec*)b.p;
    ntt.forward(x, size / 8); ntt.forward(y, size / 8);
    ntt.products(x, y, size / 8);
    ntt.inverse(x, size / 8);
    a.n = count;
    return a;
}

// Equal power-of-two lengths, product modulo x^N-1. Consumes both buffers.
template<u32 Mod = 998244353>
Buffer<u32> convolution_cyclic(Buffer<u32> a, Buffer<u32> b) {
    if (!a.n) return a;
    usize n = a.n;
    if (n < 64) {
        Buffer<u32> c(n); fill(c.p, c.p + n, 0u);
        for (usize i = 0; i < n; ++i) for (usize j = 0; j < n; ++j)
            c[(i + j) & (n - 1)] = (c[(i + j) & (n - 1)] + u64(a[i]) * b[j]) % Mod;
        return c;
    }
    const auto& ntt = convolution_detail::info<Mod>;
    auto* x = (convolution_detail::Vec*)a.p;
    auto* y = (convolution_detail::Vec*)b.p;
    ntt.forward(x, n / 8); ntt.forward(y, n / 8);
    ntt.products(x, y, n / 8); ntt.inverse(x, n / 8);
    return a;
}
}
