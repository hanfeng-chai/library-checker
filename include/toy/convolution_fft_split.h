#pragma once
#include <toy/fft_convolution.h>
namespace toy {
namespace split_fft_detail {
using namespace fft_detail;
// Signed base-2^Bits digits; the last digit keeps the remaining quotient.
// Packed=true stores even/odd coefficients as real/imaginary parts.
template<int Bits,bool Packed,class T>
void prepare(std::span<const T> input,std::span<Buffer<C4>> out){
    const auto mask=_mm256_set1_epi64x((1<<Bits)-1),half=_mm256_set1_epi64x((1<<(Bits-1))-1),base=_mm256_set1_epi64x(1<<Bits);
    auto pack=[](__m256i a,__m256i b){
        auto v=_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(a),_mm256_castsi256_ps(b),0x88));
        return _mm256_permute4x64_epi64(v,0xd8);
    };
    for(usize i=0;i<input.size();i+=8){
        alignas(32) u64 word[8]{};for(usize j=0;j<8&&i+j<input.size();++j)word[j]=input[i+j];
        auto a=_mm256_load_si256((const __m256i*)word),b=_mm256_load_si256((const __m256i*)(word+4));
        for(usize limb=0;limb<out.size();++limb){
            __m256i lo=a,hi=b;
            if(limb+1<out.size()){
                lo=_mm256_and_si256(a,mask);hi=_mm256_and_si256(b,mask);
                auto ca=_mm256_cmpgt_epi64(lo,half),cb=_mm256_cmpgt_epi64(hi,half);
                lo=_mm256_sub_epi64(lo,_mm256_and_si256(ca,base));hi=_mm256_sub_epi64(hi,_mm256_and_si256(cb,base));
                a=_mm256_sub_epi64(_mm256_srli_epi64(a,Bits),ca);b=_mm256_sub_epi64(_mm256_srli_epi64(b,Bits),cb);
            }
            auto v=pack(lo,hi);
            if constexpr(Packed){
                v=_mm256_permutevar8x32_epi32(v,_mm256_setr_epi32(0,2,4,6,1,3,5,7));
                out[limb][i/8]={_mm256_cvtepi32_pd(_mm256_castsi256_si128(v)),_mm256_cvtepi32_pd(_mm256_extracti128_si256(v,1))};
            }else{
                out[limb][i/4]={_mm256_cvtepi32_pd(_mm256_castsi256_si128(v)),_mm256_setzero_pd()};
                out[limb][i/4+1]={_mm256_cvtepi32_pd(_mm256_extracti128_si256(v,1)),_mm256_setzero_pd()};
            }
        }
    }
}
inline C4 product(C4 a,C4 b,Point w,__m256d scale){
    b.x*=scale;b.y*=scale;auto aw=a*C4::splat(w),c=a*FFT::lane<0>(b);
    c=madd(FFT::rotated<1>(a,aw),FFT::lane<1>(b),c);
    c=madd(FFT::rotated<2>(a,aw),FFT::lane<2>(b),c);
    return madd(FFT::rotated<3>(a,aw),FFT::lane<3>(b),c);
}
template<u32 P,int Bits,bool Packed,class T>
auto run(std::span<const T>a,std::span<const T>b){
    using R=std::conditional_t<P==0,u64,u32>;
    if(a.empty()||b.empty())return Buffer<R>{};
    usize count=a.size()+b.size()-1,size=std::max<usize>(32,std::bit_ceil(count)),n=size/(Packed?8:4);
    constexpr int width=P?std::bit_width(P-1):64,limbs=(width+Bits-1)/Bits,diagonals=P?2*limbs-1:limbs;
    FFT fft(n);std::array<Buffer<C4>,limbs> x,y;
    for(int i=0;i<limbs;++i){x[i]=storage<C4>(n);y[i]=storage<C4>(n);memset(x[i].p,0,n*sizeof(C4));memset(y[i].p,0,n*sizeof(C4));}
    prepare<Bits,Packed>(a,std::span(x));prepare<Bits,Packed>(b,std::span(y));
    for(int i=0;i<limbs;++i){fft.template transform<false,256>(x[i]);fft.template transform<false,256>(y[i]);}
    auto work=storage<C4>(n);Buffer<R> result(count);std::fill(result.p,result.p+count,0);
    auto conj=[](C4 a){return C4{a.x,-a.y};};auto scale=_mm256_set1_pd(1./n);
    R weight=1;
    for(int d=0;d<diagonals;++d){
        for(usize i=0;i<n;++i){
            usize j=Packed?(i?i^(std::bit_floor(i)-1):0):i;if(i>j)continue;
            auto w=fft.root[i]*fft.root[i];C4 ci{},cj{};
            for(int l=std::max(0,d-limbs+1);l<=std::min(d,limbs-1);++l){
                auto ai=x[l][i],bi=y[d-l][i];ci=ci+product(ai,bi,w,scale);
                if constexpr(Packed){
                    auto aj=x[l][j],bj=y[d-l][j],da=ai-conj(aj),db=bi-conj(bj);
                    C4 oa{da.y*_mm256_set1_pd(.5),da.x*_mm256_set1_pd(-.5)},ob{db.y*_mm256_set1_pd(.5),db.x*_mm256_set1_pd(-.5)};
                    // (E+iO)(F+iG) contains -OG. Add (1+x)OG to obtain
                    // the even/odd interleaving EF+xOG + i(EG+OF).
                    auto oo=product(oa,ob,w,scale),correction=oo+FFT::rotated<1>(oo,oo*C4::splat(w));ci=ci+correction;
                    if(i!=j)cj=cj+product(aj,bj,w.conj(),scale)+conj(correction);
                }
            }
            work[i]=ci;if constexpr(Packed)if(i!=j)work[j]=cj;
        }
        fft.template transform<true,256>(work);
        // With |x|<2^51, the 1.5*2^52 bias keeps unit spacing and rounds
        // signed integers directly into the low mantissa bits (round-to-nearest).
        auto round=[](__m256d x){return _mm256_sub_epi64(_mm256_castpd_si256(x+_mm256_set1_pd(0x1.8p52)),_mm256_set1_epi64x(0x4338000000000000ll));};
        for(usize i=0;i<count;i+=Packed?8:4){
            alignas(32) i64 values[8];
            auto e=round(work[i/(Packed?8:4)].x);
            if constexpr(Packed){
                auto o=round(work[i/8].y),lo=_mm256_unpacklo_epi64(e,o),hi=_mm256_unpackhi_epi64(e,o);
                _mm256_store_si256((__m256i*)values,_mm256_permute2x128_si256(lo,hi,0x20));
                _mm256_store_si256((__m256i*)(values+4),_mm256_permute2x128_si256(lo,hi,0x31));
            }else _mm256_store_si256((__m256i*)values,e);
            for(usize k=0;k<(Packed?8:4)&&i+k<count;++k){
                if constexpr(P){i64 r=values[k]%i64(P);if(r<0)r+=P;result[i+k]=(result[i+k]+u64(r)*weight)%P;}
                else result[i+k]+=u64(values[k])<<(Bits*d);
            }
        }
        if constexpr(P)weight=u64(weight)*(1<<Bits)%P;
    }
    return result;
}
}
// P=0 returns low 64 bits; otherwise inputs are ordinary residues modulo P.
// Floating reconstruction: tested bounds are documented alongside this header.
template<u32 P=0,bool Packed=true,class T>
auto convolution_fft_split(std::span<const T>a,std::span<const T>b){
    static_assert(P==0||(P>1&&P<(1u<<30)));
    static_assert(std::is_unsigned_v<T>&&sizeof(T)<=8);
    using R=std::conditional_t<P==0,u64,u32>;
    if(a.empty()||b.empty())return Buffer<R>{};
    if(std::min(a.size(),b.size())<=16){
        if(a.size()<b.size())std::swap(a,b);Buffer<R> c(a.size()+b.size()-1);
        for(usize k=0;k<c.n;++k){u64 sum=0;
            for(usize j=k<a.size()?0:k-a.size()+1;j<std::min(b.size(),k+1);++j)sum+=u64(a[k-j])*b[j];
            if constexpr(P)c[k]=sum%P;else c[k]=sum;
        }
        return c;
    }
    if(std::bit_ceil(a.size()+b.size())<=(1<<20))return split_fft_detail::run<P,13,Packed>(a,b);
    return split_fft_detail::run<P,10,Packed>(a,b);
}
}
