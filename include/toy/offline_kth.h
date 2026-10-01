#pragma once
#include <toy/radix_sort.h>

namespace toy {
namespace kth_detail {
inline constexpr auto prefix = [] {
    std::array<std::array<u8,8>,256> a{};
    for (int m=0;m<256;++m) { int s=0;for(int j=0;j<8;++j)a[m][j]=s+=(m>>j)&1; }
    return a;
}();
inline constexpr auto permute = [] {
    std::array<std::array<i32,8>,256> a{};
    for(int m=0;m<256;++m){int s=0;for(int j=0;j<8;++j)if(m>>j&1)a[m][s++]=j;}
    return a;
}();
inline constexpr auto store_mask = [] {
    std::array<std::array<i32,8>,9>a{};for(int n=0;n<=8;++n)for(int j=0;j<n;++j)a[n][j]=-1;return a;
}();
}
// Batch-only wavelet descent keeps a single prefix row, instead of all levels.
inline Buffer<u32> range_kth(std::span<const u32> input, Buffer<u32> left, Buffer<u32> right, Buffer<u32> rank) {
    Buffer<u32> answer(left.n); if (!left.n) return answer;
    struct Item { u32 value,index; };Buffer<Item> order(input.size());
    for(u32 i=0;i<input.size();++i)order[i]={input[i],i};
    radix_sort(std::span(order.p,order.n),[](Item x){return x.value;});
    Buffer<u32> values(input.size()),current(input.size(),input.size()+8),next(input.size(),input.size()+8);u32 count=0;
    for(usize i=0;i<order.n;++i){auto x=order[i];if(!i||x.value!=order[i-1].value)values[count++]=x.value;current[x.index]=count-1;}
    order={}; if(count==1){std::fill(answer.p,answer.p+answer.n,values[0]);return answer;}
    Buffer<u32> prefix(input.size()+1);Buffer<u8> masks((input.size()+7)/8);
    for(unsigned bit=count<=1?0:std::bit_width(count-1);bit--;){
        u32 ones=0;usize i=0;prefix[0]=0;
        for(;i+8<=input.size();i+=8){
            // The sign-bit mask indexes eight inclusive prefix counts.
            auto x=_mm256_loadu_si256((const __m256i*)(current.p+i));
            u32 mask=_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_sll_epi32(x,_mm_cvtsi32_si128(31-bit))));
            masks[i/8]=mask;
            auto sums=_mm256_cvtepu8_epi32(_mm_loadl_epi64((const __m128i*)kth_detail::prefix[mask].data()));
            _mm256_storeu_si256((__m256i*)(prefix.p+i+1),_mm256_add_epi32(sums,_mm256_set1_epi32(ones)));
            ones+=std::popcount(mask);
        }
        for(;i<input.size();++i){ones+=(current[i]>>bit)&1;prefix[i+1]=ones;}
        u32 zeros=input.size()-ones;
        if(bit){
            usize low=0,high=zeros;i=0;
            for(;i+8<=input.size();i+=8){
                auto x=_mm256_loadu_si256((const __m256i*)(current.p+i));u32 one=masks[i/8],zero=one^255;
                auto a=_mm256_permutevar8x32_epi32(x,_mm256_loadu_si256((const __m256i*)kth_detail::permute[zero].data()));
                auto b=_mm256_permutevar8x32_epi32(x,_mm256_loadu_si256((const __m256i*)kth_detail::permute[one].data()));
                u32 take=std::popcount(zero);
                // Only a zero-stream store near its boundary can overwrite
                // already-written one values. Mask that store; high has padding.
                if(zeros-low>=8)_mm256_storeu_si256((__m256i*)(next.p+low),a);
                else _mm256_maskstore_epi32((int*)(next.p+low),_mm256_loadu_si256((const __m256i*)kth_detail::store_mask[take].data()),a);
                _mm256_storeu_si256((__m256i*)(next.p+high),b);low+=take;high+=8-take;
            }
            for(;i<input.size();++i){u32 x=current[i];if(x>>bit&1)next[high++]=x;else next[low++]=x;}
            std::swap(current,next);
        }
        auto z=_mm256_set1_epi32(zeros);i=0;
        // Eight independent queries choose the zero/one side with masks.
        for(;i+8<=left.n;i+=8){
            auto l=_mm256_loadu_si256((const __m256i*)(left.p+i)),r=_mm256_loadu_si256((const __m256i*)(right.p+i)),k=_mm256_loadu_si256((const __m256i*)(rank.p+i));
            auto a=_mm256_i32gather_epi32((const int*)prefix.p,l,4),b=_mm256_i32gather_epi32((const int*)prefix.p,r,4);
            auto lz=_mm256_sub_epi32(l,a),rz=_mm256_sub_epi32(r,b),n=_mm256_sub_epi32(rz,lz),take_zero=_mm256_cmpgt_epi32(n,k);
            if(bit) {
                _mm256_storeu_si256((__m256i*)(left.p+i),_mm256_blendv_epi8(_mm256_add_epi32(z,a),lz,take_zero));
                _mm256_storeu_si256((__m256i*)(right.p+i),_mm256_blendv_epi8(_mm256_add_epi32(z,b),rz,take_zero));
                _mm256_storeu_si256((__m256i*)(rank.p+i),_mm256_sub_epi32(k,_mm256_andnot_si256(take_zero,n)));
            } else {
                // The mapped interval already agrees in every higher bit.
                auto value=_mm256_i32gather_epi32((const int*)current.p,l,4);
                auto code=_mm256_or_si256(_mm256_and_si256(value,_mm256_set1_epi32(-2)),_mm256_andnot_si256(take_zero,_mm256_set1_epi32(1)));
                _mm256_storeu_si256((__m256i*)(answer.p+i),_mm256_i32gather_epi32((const int*)values.p,code,4));
            }
        }
        for(;i<left.n;++i){
            u32 l=left[i],r=right[i],a=prefix[l],b=prefix[r],n=r-l-b+a,mask=-u32(rank[i]>=n);
            if(bit){left[i]=((l-a)&~mask)|((zeros+a)&mask);right[i]=((r-b)&~mask)|((zeros+b)&mask);rank[i]-=n&mask;}
            else answer[i]=values[(current[l]&~1u)|(mask&1)];
        }
    }
    return answer;
}
}
