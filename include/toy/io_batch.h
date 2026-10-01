#pragma once
#include <toy/io.h>
namespace toy {
// Three unsigned fields per line, bounded by 6/6/10 digits. Four independent
// line streams overlap conversion dependencies; scratch rows are concatenated.
template<class Record>void read_triple_rows(Reader& in,std::span<Record>output){
    static_assert(std::is_trivially_copyable_v<Record>);
    constexpr u32 Quarter=16384,Capacity=Quarter/6+5;Record scratch[3][Capacity];usize remaining=output.size();auto*dst=output.data();
    auto row=[](Reader& r){return Record{r.read<u32,6>(),r.read<u32,6>(),r.read<u32,10>()};};
    while(remaining>4*Quarter/6+5){Reader readers[4]{in,in,in,in};const char*end[4];Record*out[]{dst,scratch[0],scratch[1],scratch[2]};u32 count[4]{};
        for(u32 j=0;j<4;++j){auto p=in.p+(j+1)*Quarter;while(*p!='\n')++p;end[j]=p+1;if(j)readers[j].p=end[j-1];}
        for(;;){usize bytes=end[0]-readers[0].p;for(u32 j=1;j<4;++j)bytes=std::min(bytes,usize(end[j]-readers[j].p));usize steps=bytes/25;if(!steps)break;
            do{
                #pragma GCC unroll 4
                for(u32 j=0;j<4;++j)out[j][count[j]++]=row(readers[j]);
            }while(--steps);
        }
        usize used=0;for(u32 j=0;j<4;++j){while(readers[j].p<end[j])out[j][count[j]++]=row(readers[j]);if(j)memcpy(dst+used,out[j],count[j]*sizeof(*dst));used+=count[j];}dst+=used;remaining-=used;in.p=end[3];
    }
    while(remaining--)*dst++=row(in);
}
// Triples (u,v,w), with u,v < 10^6 and w < 10^10 fitting u32. Delimiter
// positions make the three conversions independent of each other's cursor.
template<class F>void read_triples(Reader& in,u32 count,F emit){
    auto number=[](const char* p,u32 digits){u64 x;u32 tail=std::min(digits,8u);memcpy(&x,p+digits-tail,8);u32 value=io_detail::decimal8(x<<((8-tail)*8));
        if(digits>8){u32 first=p[0]-'0';if(digits==10)first=first*10+p[1]-'0';value+=first*100000000;}return value;
    };
    u32 i=0;while(i<count){const char* block=in.p;auto zero=_mm256_set1_epi8('0');
        u64 mask=u32(_mm256_movemask_epi8(_mm256_sub_epi8(_mm256_loadu_si256((const __m256i*)block),zero)));
        mask|=u64(u32(_mm256_movemask_epi8(_mm256_sub_epi8(_mm256_loadu_si256((const __m256i*)(block+32)),zero))))<<32;
        while(i<count){u64 second=mask&(mask-1),third=second&(second-1);if(!third)break;
            const char* a=block+std::countr_zero(mask),*b=block+std::countr_zero(second),*c=block+std::countr_zero(third);mask=third&(third-1);
            emit(i++,number(in.p,a-in.p),number(a+1,b-a-1),number(b+1,c-b-1));in.p=c+1;
        }
    }
}
// Decode pairs of <=6-digit tokens. One delimiter mask covers several pairs;
// the callback can consume them directly without a full input array.
template<class F>void read_pairs6(Reader& in,u32 count,F emit){
    static constexpr auto shuffle=[] {
        std::array<std::array<u8,16>,64> table{};
        for(int a=1;a<=6;++a)for(int b=1;b<=6;++b){auto& t=table[a*8+b];t.fill(128);for(int j=0;j<a;++j)t[8-a+j]=j;for(int j=0;j<b;++j)t[16-b+j]=a+1+j;}return table;
    }();
    u32 i=0;while(i<count){const char* block=in.p;
        auto zero=_mm256_set1_epi8('0');u64 mask=u32(_mm256_movemask_epi8(_mm256_sub_epi8(_mm256_loadu_si256((const __m256i*)block),zero)));
        mask|=u64(u32(_mm256_movemask_epi8(_mm256_sub_epi8(_mm256_loadu_si256((const __m256i*)(block+32)),zero))))<<32;
        while(i<count&&(mask&(mask-1))){const char* space=block+std::countr_zero(mask);mask&=mask-1;const char* end=block+std::countr_zero(mask);mask&=mask-1;
            u32 a=space-in.p,b=end-space-1;auto x=_mm_sub_epi8(_mm_loadu_si128((const __m128i*)in.p),_mm_set1_epi8('0'));
            x=_mm_shuffle_epi8(x,_mm_loadu_si128((const __m128i*)shuffle[a*8+b].data()));x=_mm_maddubs_epi16(x,_mm_set1_epi16(0x010a));x=_mm_madd_epi16(x,_mm_set1_epi32(0x00010064));x=_mm_madd_epi16(_mm_packus_epi32(x,x),_mm_set1_epi32(0x00012710));
            emit(i++,u32(_mm_cvtsi128_si32(x)),u32(_mm_extract_epi32(x,1)));in.p=end+1;
        }
    }
}
// Unsigned tokens separated by exactly one space/newline, as with Reader.
template<class T,int Digits,int Chunk=16384>
void read_bulk(Reader& in,std::span<T> output){
    static_assert(T(-1)>0);
    constexpr int Quarter=Chunk/4,Capacity=Quarter/2+Digits+1;
    T region[3][Capacity];usize left=output.size();T* dst=output.data();
    auto snap=[](const char* p){while(*p>' ')++p;return p+1;};
    while(left>Chunk/2+Digits){
        Reader s[4]{in,in,in,in};const char* end[4];T* out[]{dst,region[0],region[1],region[2]};
        for(int j=0;j<4;++j)end[j]=snap(in.p+(j+1)*Quarter);
        for(int j=1;j<4;++j)s[j].p=end[j-1];
        usize counts[4]{};
        for(;;){
            usize bytes=end[0]-s[0].p;for(int j=1;j<4;++j)bytes=std::min(bytes,usize(end[j]-s[j].p));
            usize steps=bytes/(Digits+1);if(!steps)break;
            do{
                #pragma GCC unroll 4
                for(int j=0;j<4;++j)out[j][counts[j]++]=s[j].template read<T,Digits>();
            }while(--steps);
        }
        for(int j=0;j<4;++j)while(s[j].p<end[j])out[j][counts[j]++]=s[j].template read<T,Digits>();
        usize used=counts[0];for(int j=1;j<4;++j){memcpy(dst+used,out[j],counts[j]*sizeof(T));used+=counts[j];}
        dst+=used;left-=used;in.p=end[3];
    }
    while(left--)*dst++=in.template read<T,Digits>();
}
// Four independent streams; two <=9-digit tokens per stream and SIMD reduction.
inline __m256i read_two9(const char*& p){
    auto x=_mm256_loadu_si256((const __m256i*)p);
    u32 mask=_mm256_movemask_epi8(_mm256_cmpgt_epi8(_mm256_set1_epi8('0'),x));
    int n=std::countr_zero(mask),m=std::countr_zero(mask&(mask-1));
    auto y=_mm256_inserti128_si256(x,_mm_loadu_si128((const __m128i*)(p+n+1)),1);
    auto s=_mm256_inserti128_si256(_mm256_castsi128_si256(_mm_loadu_si128((const __m128i*)io_detail::shift[n].data())),
        _mm_loadu_si128((const __m128i*)io_detail::shift[m-n-1].data()),1);
    p+=m+1;y=_mm256_shuffle_epi8(_mm256_subs_epu8(y,_mm256_set1_epi8('0')),s);
    return _mm256_madd_epi16(_mm256_maddubs_epi16(y,_mm256_set1_epi16(0x010a)),_mm256_set1_epi32(0x00010064));
}
template<int Chunk=131072> void read_bulk9(Reader& in,std::span<u32> output){
    constexpr int Q=Chunk/4,Capacity=Q/2+10;
    alignas(64) static u32 scratch[3][Capacity];usize left=output.size();u32* dst=output.data();
    auto snap=[](const char* p){while(*p>' ')++p;return p+1;};
    while(left>Chunk/2+10){
        const char* s[4]{in.p},*end[4];u32* out[]{dst,scratch[0],scratch[1],scratch[2]};
        for(int j=0;j<4;++j)end[j]=snap(in.p+(j+1)*Q);
        for(int j=1;j<4;++j)s[j]=end[j-1];usize count=0;
        for(;;){
            usize bytes=end[0]-s[0];for(int j=1;j<4;++j)bytes=std::min(bytes,usize(end[j]-s[j]));
            usize steps=bytes/20;if(!steps)break;
            do{
                auto a=read_two9(s[0]),b=read_two9(s[1]),c=read_two9(s[2]),d=read_two9(s[3]);
                auto k=_mm256_set1_epi32(0x00012710);
                auto ab=_mm256_castsi256_ps(_mm256_madd_epi16(_mm256_packus_epi32(a,b),k));
                auto cd=_mm256_castsi256_ps(_mm256_madd_epi16(_mm256_packus_epi32(c,d),k));
                auto hi=_mm256_castps_si256(_mm256_shuffle_ps(ab,cd,0x88)),lo=_mm256_castps_si256(_mm256_shuffle_ps(ab,cd,0xdd));
                auto v=_mm256_add_epi32(_mm256_mullo_epi32(hi,_mm256_set1_epi32(100000000)),lo);
                auto q=_mm256_permutevar8x32_epi32(v,_mm256_setr_epi32(0,4,1,5,2,6,3,7));
                auto x=_mm256_castsi256_si128(q),y=_mm256_extracti128_si256(q,1);
                _mm_storel_epi64((__m128i*)(out[0]+count),x);_mm_storeh_pd((double*)(out[1]+count),_mm_castsi128_pd(x));
                _mm_storel_epi64((__m128i*)(out[2]+count),y);_mm_storeh_pd((double*)(out[3]+count),_mm_castsi128_pd(y));
                count+=2;
            }while(--steps);
        }
        usize used=0;
        for(int j=0;j<4;++j){
            usize n=count;Reader r=in;r.p=s[j];while(r.p<end[j])out[j][n++]=r.read<u32,9>();
            if(j)memcpy(dst+used,out[j],n*4);used+=n;
        }
        in.p=end[3];dst+=used;left-=used;
    }
    while(left--)*dst++=in.read<u32,9>();
}
namespace bulk_detail {
// Aiyiyi's SWAR decimal formatter: eight numbers, nine columns plus separator.
template<int G>
[[gnu::always_inline]] inline void blocks3(const uint32_t* src, char* p) {
    const __m256i m1 = _mm256_set1_epi64x(3518437209LL), m8 = _mm256_set1_epi64x(720575941LL);
    const __m256i k4 = _mm256_set1_epi32(10000);
    const __m256i K1 = _mm256_set1_epi32(0x0000FF00), K2 = _mm256_set1_epi32(0x00FF0000), K3 = _mm256_set1_epi32(int(0xFF000000u));
    __m256i x[G], S1[G], S2[G], A[G];
    for (int g = 0; g < G; ++g) x[g] = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(src + 8 * g));
    for (int g = 0; g < G; ++g) {
        const __m256i xo = _mm256_srli_epi64(x[g], 32);
        const __m256i q = _mm256_blend_epi32(_mm256_srli_epi64(_mm256_mul_epu32(x[g], m1), 45),
                                             _mm256_srli_epi64(_mm256_mul_epu32(xo, m1), 13), 0xAA);
        const __m256i a = _mm256_blend_epi32(_mm256_srli_epi64(_mm256_mul_epu32(x[g], m8), 56),
                                             _mm256_srli_epi64(_mm256_mul_epu32(xo, m8), 24), 0xAA);
        const __m256i c = _mm256_sub_epi32(x[g], _mm256_mullo_epi32(q, k4));
        const __m256i b = _mm256_sub_epi32(q, _mm256_mullo_epi32(a, k4));
        auto group = [](__m256i y) {
            const __m256i h = _mm256_srli_epi16(_mm256_mulhi_epu16(y, _mm256_set1_epi16(5243)), 3);
            const __m256i hl = _mm256_add_epi32(y, _mm256_mullo_epi32(h, _mm256_set1_epi32(65436)));
            const __m256i t = _mm256_mulhi_epu16(hl, _mm256_set1_epi16(6554));
            return _mm256_add_epi16(hl, _mm256_mullo_epi16(t, _mm256_set1_epi16(246)));
        };
        auto lt = [&](int k) { return _mm256_cmpgt_epi32(_mm256_set1_epi32(k), x[g]); };
        const __m256i Mb = _mm256_blendv_epi8(_mm256_blendv_epi8(_mm256_blendv_epi8(lt(10000), lt(100000), K1), lt(1000000), K2),
                                              lt(10000000), K3);
        const __m256i Mc = _mm256_blendv_epi8(_mm256_blendv_epi8(_mm256_and_si256(lt(10), K1), lt(100), K2), lt(1000), K3);
        const __m256i z = _mm256_set1_epi8('0'), s = _mm256_set1_epi8(' ');
        const __m256i Gb = _mm256_blendv_epi8(_mm256_add_epi8(group(b), z), s, Mb);
        const __m256i Gc = _mm256_blendv_epi8(_mm256_add_epi8(group(c), z), s, Mc);
        S1[g] = _mm256_unpacklo_epi32(Gb, Gc);   // [b0 c0 b1 c1 | b4 c4 b5 c5]
        S2[g] = _mm256_unpackhi_epi32(Gb, Gc);
        A[g] = _mm256_or_si256(
            _mm256_blendv_epi8(_mm256_add_epi32(a, _mm256_set1_epi32('0')), _mm256_set1_epi32(' '), lt(100000000)),
            _mm256_set1_epi32(int(uint32_t(' ') << 24)));
    }
    const __m256i dlo = _mm256_setr_epi8(-128, 3, 2, 1, 0, 7, 6, 5, 4, -128, -128, -128, -128, -128, -128, -128,
                                         -128, 3, 2, 1, 0, 7, 6, 5, 4, -128, -128, -128, -128, -128, -128, -128);
    const __m256i dhi = _mm256_add_epi8(dlo, _mm256_setr_epi8(0, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0,
                                                             0, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0));
    auto actl = [](int k) {
        const char s = char(4 * k), e = char(4 * k + 3), z = char(-128);
        return _mm256_setr_epi8(s, z, z, z, z, z, z, z, z, e, z, z, z, z, z, z, s, z, z, z, z, z, z, z, z, e, z, z, z, z, z, z);
    };
    for (int g = 0; g < G; ++g) {
        char* const o = p + 80 * g;
        const __m256i f0 = _mm256_or_si256(_mm256_shuffle_epi8(S1[g], dlo), _mm256_shuffle_epi8(A[g], actl(0)));
        const __m256i f1 = _mm256_or_si256(_mm256_shuffle_epi8(S1[g], dhi), _mm256_shuffle_epi8(A[g], actl(1)));
        const __m256i f2 = _mm256_or_si256(_mm256_shuffle_epi8(S2[g], dlo), _mm256_shuffle_epi8(A[g], actl(2)));
        const __m256i f3 = _mm256_or_si256(_mm256_shuffle_epi8(S2[g], dhi), _mm256_shuffle_epi8(A[g], actl(3)));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(o + 0), _mm256_castsi256_si128(f0));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(o + 10), _mm256_castsi256_si128(f1));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(o + 20), _mm256_castsi256_si128(f2));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(o + 30), _mm256_castsi256_si128(f3));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(o + 40), _mm256_extracti128_si256(f0, 1));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(o + 50), _mm256_extracti128_si256(f1, 1));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(o + 60), _mm256_extracti128_si256(f2, 1));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(o + 70), _mm256_extracti128_si256(f3, 1));
    }
}
}
// Values <10^9. Nine columns (spaces on the left), then a separator. Tokens
// are canonical decimal; use this only where repeated whitespace is accepted.
template<usize N> void write_bulk9(Writer<N>& out,std::span<const u32> a){
    static_assert(N>=326);
    usize i=0;
    for(;i+32<=a.size();i+=32){if(out.buf+N-out.p<326)out.flush();bulk_detail::blocks3<4>(a.data()+i,out.p);out.p+=320;}
    for(;i+8<=a.size();i+=8){if(out.buf+N-out.p<86)out.flush();bulk_detail::blocks3<1>(a.data()+i,out.p);out.p+=80;}
    for(;i<a.size();++i)out.write(a[i],' ');
}
// Canonical decimal output for x<10^6: one leading group and three fixed digits.
template<usize N> [[gnu::always_inline]] inline char* format6(char* p,u32 x,char end){
    u32 high=x/1000,low=x-high*1000;
    if(high){p=Writer<N>::leading(p,high);u32 digits=std::bit_cast<u32>(io_detail::digits[low])>>8;memcpy(p,&digits,4);p+=3;}
    else p=Writer<N>::leading(p,x);
    *p++=end;return p;
}
template<usize N> [[gnu::always_inline]] inline void write6(Writer<N>& out,u32 x,char end='\n'){
    if(out.buf+N-out.p<8)out.flush();out.p=format6<N>(out.p,x,end);
}
template<usize N,class T,usize Extent>void write_bulk6(Writer<N>& out,std::span<T,Extent> values,char end=' '){
    for(usize i=0;i<values.size();){usize stop=std::min(values.size(),i+usize(out.buf+N-out.p)/8);if(stop==i){out.flush();continue;}
        char* p=out.p;for(;i<stop;++i)p=format6<N>(p,values[i],end);out.p=p;}
}

}
