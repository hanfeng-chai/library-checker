#pragma once
#include <toy/convolution.h>
#include <toy/mod.h>
#ifndef TOY_NTT_ASM
#define TOY_NTT_ASM 1
#endif
#if TOY_NTT_ASM
#include <toy/ntt_asm.h>
#endif

namespace toy {
template<class T> Buffer<T> ntt_storage(usize n,usize capacity=0){
    usize count=std::max(n,capacity);if(count*sizeof(T)<(1<<20))return Buffer<T>(n,capacity);
    usize bytes=(count*sizeof(T)+(1<<21)-1)&-usize(1<<21);Buffer<T> a;
    a.p=(T*)aligned_alloc(1<<21,bytes);a.n=n;a.capacity=count;madvise(a.p,bytes,MADV_HUGEPAGE);return a;
}

// Cache-resident partial NTT. The radix-4 schedule follows Aiyiyi's qasm
// submission; fixed multipliers use Shoup, and 8-point products use Montgomery.
template<u32 P, int Tile = 256>
struct ProductNTT {
    static_assert(P<(1u<<30) && (P-1)%8==0);
    using V=__m256i;using M=Mod<P>;
    // Eight values followed by their eight Shoup quotients, in 64-byte blocks.
    struct Root {u32 w,q;};
    Buffer<u32> roots, inverse;
    static usize index(usize k){return (k/8)*16+k%8;}
    static Root get(const u32* r,usize k){k=index(k);return {r[k],r[k+8]};}
    static constexpr u32 generator=[] {u32 g=2;while(M::pow(g,(P-1)/2)!=P-1)++g;return g;}();
    static V splat(u32 x){return _mm256_set1_epi32(x);}
    static V add(V x,V y){return _mm256_add_epi32(x,y);}
    static V sub(V x,V y){return _mm256_sub_epi32(x,y);}
    static V low(V x){return _mm256_min_epu32(x,sub(x,splat(2*P)));}
    static V shrink(V x){return _mm256_min_epu32(x,sub(x,splat(P)));}
    static V canon(V x){return shrink(low(x));}
    static V diff(V x,V y){return add(x,sub(splat(2*P),y));}
    struct Fixed {
        V w,q;
        Fixed(Root r):w(splat(r.w)),q(splat(r.q)){}
        [[gnu::always_inline]] V operator()(V x)const {
            auto hi=_mm256_srli_epi64(_mm256_mul_epu32(x,q),32);
            auto odd=_mm256_mul_epu32(_mm256_srli_epi64(x,32),q);
            auto estimate=_mm256_blend_epi32(hi,odd,0xaa);
            auto r=sub(_mm256_mullo_epi32(x,w),_mm256_mullo_epi32(estimate,splat(P)));
            // Keep the two subtracts from being reassociated across a butterfly.
#if TOY_NTT_ASM
            asm("":"+x"(r));
#endif
            return r;
        }
    };
    static Root fixed(u32 w){return {w,u32((u64(w)<<32)/P)};}
    explicit ProductNTT(usize size):roots(ntt_storage<u32>(std::max<usize>(16,size/8))),inverse(ntt_storage<u32>(std::max<usize>(16,size/8))) {
        roots[0]=inverse[0]=1;roots[8]=inverse[8]=fixed(1).q;
        Fixed to_mont(fixed((u64(1)<<32)%P));
        for(usize h=1;h<size/16;h*=2){
            u32 w=M::pow(generator,(P-1)/(4*h)),iw=M::pow(w,P-2);
            for(int dir=0;dir<2;++dir){
                u32* r=dir?inverse.p:roots.p;u32 factor=dir?iw:w;
                if(h<8)for(usize j=0;j<h;++j){Root z=fixed(M::mul(r[index(j)],factor));r[index(h+j)]=z.w;r[index(h+j)+8]=z.q;}
                else{
                    Fixed f(fixed(factor));
                    for(usize j=0;j<h;j+=8){
                        V z=shrink(f(_mm256_load_si256((V*)(r+2*j))));
                        V q=_mm256_mullo_epi32(shrink(to_mont(z)),splat(M::inverse));
                        _mm256_store_si256((V*)(r+2*(h+j)),z);_mm256_store_si256((V*)(r+2*(h+j)+8),q);
                    }
                }
            }
        }
    }
    // Forward: A=a+xc, C=a-xc, B=b+xd, D=b-xd;
    // outputs A+yB, A-yB, C+zD, C-zD. Ranges: forward <4P, inverse <2P.
    template<bool Inv,bool Identity>
    [[gnu::always_inline]] void butterfly(V* f,usize h,usize k)const {
        const u32* r=Inv?inverse.p:roots.p;
        Fixed x(get(r,k)),y(get(r,2*k)),z(get(r,2*k+1));
        for(usize i=0;i<h;++i){
            V a=f[i],b=f[i+h],c=f[i+2*h],d=f[i+3*h];
            if constexpr(!Inv){
                a=low(a);b=low(b);
                if constexpr(Identity){c=low(c);d=low(d);}else{c=x(c);d=x(d);}
                V ac=low(add(a,c)),amc=low(diff(a,c)),bd=add(b,d),bmd=diff(b,d);
                if constexpr(Identity)bd=low(bd);else bd=y(bd);
                bmd=z(bmd);
                f[i]=add(ac,bd);f[i+h]=diff(ac,bd);f[i+2*h]=add(amc,bmd);f[i+3*h]=diff(amc,bmd);
            } else {
                V ab=low(add(a,b)),cd=low(add(c,d)),amb=diff(a,b),cmd=diff(c,d);
                if constexpr(Identity)amb=low(amb);else amb=y(amb);
                cmd=z(cmd);
                V o0=low(add(ab,cd)),o1=low(add(amb,cmd)),o2=diff(ab,cd),o3=diff(amb,cmd);
                if constexpr(Identity){o2=low(o2);o3=low(o3);}else{o2=x(o2);o3=x(o3);}
                f[i]=o0;f[i+h]=o1;f[i+2*h]=o2;f[i+3*h]=o3;
            }
        }
    }
    template<bool Inv,int H=0>
    [[gnu::always_inline]] void group(V* a,V* b,usize h,usize k)const {
        if constexpr(H)h=H;
#if TOY_NTT_ASM
        if constexpr(H==0 || H>=4) if(k && h>=4){
            const u32* r=Inv?inverse.p:roots.p;
            const u32 *x=r+index(k),*y=r+index(2*k);
            if constexpr(Inv)ButterflyAsm<P>::inv_at71(a,h,x,y);
            else if constexpr(H==4)ButterflyAsm<P>::fwd_at104(a,b,h,x,y);
            else{ButterflyAsm<P>::fwd_ls2h(a,h,x,y);ButterflyAsm<P>::fwd_ls2h(b,h,x,y);}
            return;
        }
#endif
        if(k==0){butterfly<Inv,true>(a,h,k);if constexpr(!Inv)butterfly<false,true>(b,h,k);}
        else{butterfly<Inv,false>(a,h,k);if constexpr(!Inv)butterfly<false,false>(b,h,k);}
    }
    // Each row is [w*a, a]. Sliding eight-word windows implement multiplication
    // modulo x^8-w; four independent leaves hide the multiply latency.
    struct alignas(64) Leaf {u32 window[4][16],coeff[4][8];};
    [[gnu::always_inline]] void prepare_leaf(V* __restrict a,V* __restrict b,usize k,Leaf& work)const{
        Root w=get(roots.p,2*k),iw=get(roots.p,2*k+1);Root weights[]{w,{P-w.w,~w.q},iw,{P-iw.w,~iw.q}};
        for(int t=0;t<4;++t){V x=canon(a[t]);
            _mm256_store_si256((V*)work.window[t],shrink(Fixed(weights[t])(x)));
            _mm256_store_si256((V*)(work.window[t]+8),x);
            _mm256_store_si256((V*)work.coeff[t],canon(b[t]));
        }
    }
    [[gnu::always_inline]] void multiply_leaf(V*a,const Leaf& work)const{
        V even[4]{},odd[4]{};
        #pragma GCC unroll 2
        for(int i=0;i<8;++i){
            #pragma GCC unroll 4
            for(int t=0;t<4;++t){
                V x=_mm256_loadu_si256((const V*)(work.window[t]+8-i)),y=splat(work.coeff[t][i]);
                even[t]=_mm256_add_epi64(even[t],_mm256_mul_epu32(x,y));
                odd[t]=_mm256_add_epi64(odd[t],_mm256_mul_epu32(i?_mm256_loadu_si256((const V*)(work.window[t]+9-i)):_mm256_srli_epi64(x,32),y));
            }
        }
        for(int t=0;t<4;++t)a[t]=low(convolution_detail::reduce(even[t],odd[t],splat(M::inverse),splat(P)));
    }
    void leaf_weights(usize k,u32* w)const{
        Root x=get(roots.p,2*k),y=get(roots.p,2*k+1);
        w[0]=x.w;w[1]=P-x.w;w[2]=y.w;w[3]=P-y.w;
        w[4]=x.q;w[5]=~x.q;w[6]=y.q;w[7]=~y.q;
    }
    template<int NV,int H>
    [[gnu::always_inline]] void forward(V*a,V*b,usize k)const{
        if constexpr(H>=4){
            for(int i=0;i<NV;i+=4*H)group<false,H>(a+i,b+i,H,k*(NV/(4*H))+i/(4*H));
            forward<NV,H/4>(a,b,k);
        }
    }
    template<int NV,int H>
    [[gnu::always_inline]] void backward(V*a,usize k)const{
        if constexpr(H<NV){
            for(int i=0;i<NV;i+=4*H)group<true,H>(a+i,nullptr,H,k*(NV/(4*H))+i/(4*H));
            backward<NV,H*4>(a,k);
        }
    }
    template<int NV> [[gnu::noinline]] void tile(V*a,V*b,usize k)const{
        forward<NV,NV/4>(a,b,k);
#if TOY_NTT_ASM
        // Prepare the next group while multiplying the previous one. Only the
        // repeated middle step needs assembly; tile endpoints use C++.
        Leaf work[2];alignas(32) u32 weights[2][8];
        usize first=k*NV/4;leaf_weights(first,weights[0]);
        group<false,1>(a,b,1,first);prepare_leaf(a,b,first,work[0]);
        for(int j=0;j<NV;j+=4){
            int cur=j/4&1;usize kc=first+j/4;
            if(j+4<NV){
                usize kn=kc+1;leaf_weights(kn,weights[cur^1]);
                ButterflyAsm<P>::bottom23_s12(a+j+4,b+j+4,&work[cur^1],roots.p+index(kn),roots.p+index(2*kn),weights[cur^1],
                    a+j,&work[cur],inverse.p+index(kc),inverse.p+index(2*kc));
            }else{multiply_leaf(a+j,work[cur]);group<true,1>(a+j,nullptr,1,kc);}
        }
#else
        Leaf work[2];
        auto prepare=[&](int j){usize index=(k*NV+j)/4;group<false,1>(a+j,b+j,1,index);prepare_leaf(a+j,b+j,index,work[j/4&1]);};
        prepare(0);
        for(int j=0;j<NV;j+=4){
            if(j+4<NV)prepare(j+4);
            multiply_leaf(a+j,work[j/4&1]);group<true,1>(a+j,nullptr,1,(k*NV+j)/4);
        }
#endif
        backward<NV,4>(a,k);
    }
    void visit(V*a,V*b,usize n,usize k)const{
        if(n==4)tile<4>(a,b,k);
        else if(n==16)tile<16>(a,b,k);
        else if(n==64)tile<64>(a,b,k);
        else if(n==Tile)tile<Tile>(a,b,k);
        else{
            usize h=n/4;group<false>(a,b,h,k);
            for(usize i=0;i<4;++i)visit(a+i*h,b+i*h,h,k*4+i);
            group<true>(a,nullptr,h,k);
        }
    }
    void run(u32* aa,u32* bb,usize size,usize na,usize nb)const{
        V*a=(V*)aa,*b=(V*)bb;usize n=size/8;
        Fixed scale(fixed(M::mul((u64(1)<<32)%P,M::pow(n,P-2))));
        if(std::countr_zero(n)&1){
            usize h=n/2;
            auto top=[&](V*f,usize count){
                if(count<=size/2){for(usize i=0;i<h;++i)f[i+h]=f[i];}
                else for(usize i=0;i<h;++i){V x=f[i],y=f[i+h];f[i]=low(add(x,y));f[i+h]=low(diff(x,y));}
            };
            top(a,na);top(b,nb);visit(a,b,h,0);visit(a+h,b+h,h,1);
            for(usize i=0;i<h;++i){V x=a[i],y=a[i+h];a[i]=shrink(scale(add(x,y)));a[i+h]=shrink(scale(diff(x,y)));}
        }else{
            usize h=n/4;Fixed z(get(roots.p,1)),iz(get(inverse.p,1));
            auto top=[&](V*f,usize count){
                if(count>size/2){butterfly<false,true>(f,h,0);return;}
                for(usize i=0;i<h;++i){V x=f[i],y=f[i+h],zy=z(y);
                    f[i]=add(x,y);f[i+h]=diff(x,y);f[i+2*h]=add(x,zy);f[i+3*h]=diff(x,zy);}
            };
            top(a,na);top(b,nb);for(usize i=0;i<4;++i)visit(a+i*h,b+i*h,h,i);
            for(usize i=0;i<h;++i){
                V p=a[i],q=a[i+h],r=a[i+2*h],s=a[i+3*h];
                V ab=low(add(p,q)),cd=low(add(r,s)),amb=low(diff(p,q)),cmd=iz(diff(r,s));
                a[i]=shrink(scale(add(ab,cd)));a[i+h]=shrink(scale(add(amb,cmd)));
                a[i+2*h]=shrink(scale(diff(ab,cd)));a[i+3*h]=shrink(scale(diff(amb,cmd)));
            }
        }
    }
};
// Consumes canonical coefficients. P is prime; padded length N needs N/8-th
// roots. Separate spectra remain available from convolution_detail::info<P>.
template<u32 P=998244353,int Tile=256>
Buffer<u32> convolution_fast(Buffer<u32>a,Buffer<u32>b){
    if(!a.n||!b.n)return {};
    if(std::min(a.n,b.n)<=16)return convolution<P>(std::move(a),std::move(b));
    usize na=a.n,nb=b.n,count=na+nb-1,size=std::max<usize>(64,std::bit_ceil(count));
    auto pad=[&](Buffer<u32>& f){
        f.reserve(size+16);usize end=f.n<=size/2?size/2:size;
        std::fill(f.p+f.n,f.p+end,0);f.n=size;
    };
    pad(a);pad(b);
    ProductNTT<P,Tile> job(size);job.run(a.p,b.p,size,na,nb);a.n=count;return a;
}
}
