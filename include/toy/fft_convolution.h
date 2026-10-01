#pragma once
#include <toy/convolution_crt.h>

namespace toy {
namespace fft_detail {
template<class T> Buffer<T> storage(usize n) {
    if(n*sizeof(T)<(1<<20))return Buffer<T>(n);
    usize bytes=(n*sizeof(T)+(1<<21)-1)&-usize(1<<21);
    Buffer<T> a;
    a.p=(T*)aligned_alloc(1<<21,bytes);a.n=a.capacity=n;
    madvise(a.p,bytes,MADV_HUGEPAGE);
    return a;
}
struct Point {
    f64 x, y;
    Point operator*(Point b) const { return {fma(x,b.x,-y*b.y),fma(x,b.y,y*b.x)}; }
    Point conj() const { return {x,-y}; }
};
struct C4 {
    __m256d x{}, y{};
    C4 operator+(C4 b) const { return {x+b.x,y+b.y}; }
    C4 operator-(C4 b) const { return {x-b.x,y-b.y}; }
    C4 operator*(C4 b) const { return {_mm256_fmsub_pd(x,b.x,y*b.y),_mm256_fmadd_pd(x,b.y,y*b.x)}; }
    C4 times_i() const { return {-y,x}; }
    static C4 splat(Point p) { return {_mm256_set1_pd(p.x),_mm256_set1_pd(p.y)}; }
};
inline C4 madd(C4 a,C4 b,C4 c) {
    return {_mm256_fmadd_pd(a.x,b.x,_mm256_fnmadd_pd(a.y,b.y,c.x)),
            _mm256_fmadd_pd(a.x,b.y,_mm256_fmadd_pd(a.y,b.x,c.y))};
}
struct FFT {
    Buffer<Point> root;
    explicit FFT(usize vectors):root(storage<Point>(vectors)) {
        root[0]={1,0};
        for(usize half=1;half<vectors;half*=2) {
            f80 angle=std::numbers::pi_v<f80>/(2*half);
            Point w={f64(cosl(angle)),f64(sinl(angle))};
            for(usize i=0;i<half;++i)root[half+i]=root[i]*w;
        }
    }
    template<bool Inverse, usize Cache = 1024> void transform(Buffer<C4>& a) const {
        usize n=a.n,limit=n>>(std::countr_zero(n)&1);
        auto top=[&]{for(usize i=0;i<n/2;++i){auto x=a[i],y=a[i+n/2];a[i]=x+y;a[i+n/2]=x-y;}};
        if constexpr(!Inverse)if(limit!=n)top();
        auto stage=[&](usize begin,usize end,usize len) {
            usize q=len/4;
            for(usize s=begin,k=begin>>std::countr_zero(len);s<end;s+=len,++k) {
                Point p1=root[k],p2=root[2*k],p3=p1*p2;
                if constexpr(Inverse)p1=p1.conj(),p2=p2.conj(),p3=p3.conj();
                C4 r1=C4::splat(p1),r2=C4::splat(p2),r3=C4::splat(p3);
                for(usize j=0;j<q;++j) {
                    auto x0=a[s+j],x1=a[s+j+q],x2=a[s+j+2*q],x3=a[s+j+3*q];
                    if constexpr(Inverse) {
                        auto u=x0+x1,v=x2+x3,w=x0-x1,t=(x3-x2).times_i();
                        a[s+j]=u+v;a[s+j+q]=(w+t)*r2;a[s+j+2*q]=(u-v)*r1;a[s+j+3*q]=(w-t)*r3;
                    } else {
                        x1=x1*r2;x2=x2*r1;x3=x3*r3;
                        auto u=x0+x2,v=x0-x2,w=x1+x3,t=(x1-x3).times_i();
                        a[s+j]=u+w;a[s+j+q]=u-w;a[s+j+2*q]=v+t;a[s+j+3*q]=v-t;
                    }
                }
            }
        };
        // Finish each cache-sized subtree before moving to its neighbours.
        auto run=[&](auto&& self,usize start,usize size)->void {
            if(size<=Cache) {
                for(usize len=Inverse?4:size;len>=4&&len<=size;len=Inverse?len*4:len/4)
                    stage(start,start+size,len);
                return;
            }
            if constexpr(!Inverse)stage(start,start+size,size);
            for(usize i=0;i<4;++i)self(self,start+i*size/4,size/4);
            if constexpr(Inverse)stage(start,start+size,size);
        };
        for(usize start=0;start<n;start+=limit)run(run,start,limit);
        if constexpr(Inverse)if(limit!=n)top();
    }
    template<int J> static C4 rotated(C4 a,C4 aw) {
        constexpr int mask=((1<<J)-1)<<(4-J),perm=J==1?0x93:J==2?0x4e:0x39;
        return {_mm256_permute4x64_pd(_mm256_blend_pd(a.x,aw.x,mask),perm),
                _mm256_permute4x64_pd(_mm256_blend_pd(a.y,aw.y,mask),perm)};
    }
    template<int J> static C4 lane(C4 a) {
        return {_mm256_permute4x64_pd(a.x,J*0x55),_mm256_permute4x64_pd(a.y,J*0x55)};
    }
    void product(Buffer<C4>& a,const Buffer<C4>& b) const {
        auto scale=_mm256_set1_pd(1./a.n);
        // Each leaf is a polynomial modulo x^4-w. Rotation multiplies just the
        // wrapped coefficients by w; all four result coefficients occupy lanes.
        for(usize i=0;i<a.n;++i) {
            auto x=a[i],y=b[i];y.x*=scale;y.y*=scale;
            auto xw=x*C4::splat(root[i]*root[i]);
            auto c=x*lane<0>(y);
            c=madd(rotated<1>(x,xw),lane<1>(y),c);
            c=madd(rotated<2>(x,xw),lane<2>(y),c);
            a[i]=madd(rotated<3>(x,xw),lane<3>(y),c);
        }
    }
};
template<u32 P> constexpr u32 sqrt_mod(u32 x) {
    using M=Mod<P>;
    int s=std::countr_zero(P-1);u32 q=(P-1)>>s,z=2;
    while(M::pow(z,(P-1)/2)==1)++z;
    u32 c=M::pow(z,q),r=M::pow(x,(q+1)/2),t=M::pow(x,q);
    while(t!=1) {
        int i=1;u32 u=M::mul(t,t);while(u!=1)u=M::mul(u,u),++i;
        u32 b=M::pow(c,1u<<(s-i-1));r=M::mul(r,b);c=M::mul(b,b);t=M::mul(t,c);s=i;
    }
    return r;
}
struct Basis { i64 a,b,c,e;u32 d,root; };
// Reduce the zero lattice (P,0),(-root,1) in the norm re^2+d*im^2.
// Both basis vectors map to zero under re+root*im modulo P.
template<u32 P> inline constexpr Basis basis=[] {
    using M=Mod<P>;u32 d=1;
    while(M::pow(P-d,(P-1)/2)!=1)++d;
    u32 r=sqrt_mod<P>(P-d);
    i128 a=P,b=0,c=-i128(r),e=1;
    auto norm=[&](i128 x,i128 y){return x*x+d*y*y;};
    while(true) {
        if(norm(a,b)>norm(c,e))std::swap(a,c),std::swap(b,e);
        i128 dot=a*c+d*b*e,len=norm(a,b),q=(2*dot+(dot>=0?len:-len))/(2*len);
        if(!q||norm(c-q*a,e-q*b)>=norm(c,e))break;
        c-=q*a;e-=q*b;
    }
    return Basis{i64(a),i64(b),i64(c),i64(e),d,r};
}();
inline __m256d nearest(__m256d x){return _mm256_round_pd(x,_MM_FROUND_TO_NEAREST_INT|_MM_FROUND_NO_EXC);}
}

// P is an odd prime below 2^30. Floating-point reconstruction is tested up to
// 2^20 padded coefficients; larger products and short sides use exact CRT.
// The quadratic-lattice embedding follows the repository Rohan_Kapri/adamant
// submissions; this kernel uses four-coefficient leaves and a small cache tree.
template<u32 P> Buffer<u32> convolution_fft(std::span<const u32> a,std::span<const u32> b) {
    using namespace fft_detail;
    if(a.empty()||b.empty())return {};
    usize count=a.size()+b.size()-1,n=std::bit_ceil(count);
    if(std::min(a.size(),b.size())<=16||n>(1<<20))return convolution_crt<P>(a,b);
    constexpr auto L=basis<P>;
    constexpr f64 det=f64(L.a)*L.e-f64(L.c)*L.b;
    f64 scale=sqrt(f64(L.d));
    FFT fft(n/4);auto x=storage<C4>(n/4),y=storage<C4>(n/4);
    auto fill=[&](Buffer<C4>& target,std::span<const u32> input,u32 seed) {
        usize i=0;
        for(;i<input.size();i+=4) {
            alignas(16) u32 tail[4]{};
            __m128i bits;
            if(i+4<=input.size())bits=_mm_loadu_si128((const __m128i*)(input.data()+i));
            else {memcpy(tail,input.data()+i,(input.size()-i)*4);bits=_mm_load_si128((const __m128i*)tail);}
            auto v=_mm256_cvtepi32_pd(bits);
            // Deterministic stochastic rounding spreads correlated error on
            // constant inputs while keeping benchmark runs reproducible.
            auto hash=_mm_xor_si128(_mm_set_epi32(i+3,i+2,i+1,i),_mm_set1_epi32(seed));
            hash=_mm_mullo_epi32(hash,_mm_set1_epi32(0x9e3779b1));hash=_mm_xor_si128(hash,_mm_srli_epi32(hash,15));
            hash=_mm_mullo_epi32(hash,_mm_set1_epi32(0x85ebca77));hash=_mm_xor_si128(hash,_mm_srli_epi32(hash,13));
            auto noise=_mm256_cvtepi32_pd(hash)*_mm256_set1_pd(0x1p-32);
            auto q=nearest(_mm256_fmadd_pd(v,_mm256_set1_pd(L.e/det),noise));
            auto t=nearest(_mm256_fmadd_pd(v,_mm256_set1_pd(-L.b/det),noise));
            auto re=_mm256_fnmadd_pd(q,_mm256_set1_pd(L.a),_mm256_fnmadd_pd(t,_mm256_set1_pd(L.c),v));
            auto im=_mm256_fnmadd_pd(q,_mm256_set1_pd(L.b),-t*_mm256_set1_pd(L.e));
            target[i/4]={re,im*_mm256_set1_pd(scale)};
        }
        std::fill(target.p+(i+3)/4,target.p+target.n,C4{});
    };
    fill(x,a,0x21a772c9);fill(y,b,0x6172f20b);
    fft.transform<false>(x);fft.transform<false>(y);fft.product(x,y);fft.transform<true>(x);
    Buffer<u32> c(count);
    const f64 lc=abs(L.b)>=abs(L.e)?L.a:L.c,le=abs(L.b)>=abs(L.e)?L.b:L.e;
    for(usize i=0;i<count;i+=4) {
        // Round the ring coordinates, then subtract a lattice vector to shrink
        // the imaginary part before projecting back to the modular residue.
        auto re=nearest(x[i/4].x),im=nearest(x[i/4].y*_mm256_set1_pd(1/scale));
        auto q=nearest(im*_mm256_set1_pd(1/le));
        auto u=_mm256_fnmadd_pd(q,_mm256_set1_pd(lc),re),v=_mm256_fnmadd_pd(q,_mm256_set1_pd(le),im);
        auto h=_mm256_fmadd_pd(v,_mm256_set1_pd(L.root),u);
        auto r=_mm256_fnmadd_pd(nearest(h*_mm256_set1_pd(1./P)),_mm256_set1_pd(P),h);
        r+=_mm256_and_pd(_mm256_cmp_pd(r,_mm256_setzero_pd(),_CMP_LT_OQ),_mm256_set1_pd(P));
        auto values=_mm256_cvttpd_epi32(r);
        if(i+4<=count)_mm_storeu_si128((__m128i*)(c.p+i),values);
        else {alignas(16)u32 tail[4];_mm_store_si128((__m128i*)tail,values);memcpy(c.p+i,tail,(count-i)*4);}
    }
    return c;
}
}
