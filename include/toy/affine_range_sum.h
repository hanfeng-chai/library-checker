#pragma once
#include <toy/affine.h>
#include <toy/buffer.h>
#include <toy/montgomery.h>
namespace toy {
template<u32 P=998244353> struct AffineRangeSum {
    using M=Mod<P>;using A=Montgomery<P>;
    struct Factor {
        __m256i value, quotient;
        explicit Factor(u32 x):value(_mm256_set1_epi32(x)),quotient(_mm256_set1_epi32((u64(x)<<32)/P)){}
        __m256i operator()(__m256i x)const{
            auto even=_mm256_mul_epu32(x,quotient),odd=_mm256_mul_epu32(_mm256_srli_epi64(x,32),quotient);
            auto q=_mm256_blend_epi32(_mm256_srli_epi64(even,32),odd,0xaa);
            return _mm256_sub_epi32(_mm256_mullo_epi32(x,value),_mm256_mullo_epi32(q,_mm256_set1_epi32(P)));
        }
    };
    struct alignas(64) Node {u32 sums[16],a[16],b[16];};
    Buffer<u32> sums;Buffer<Node> nodes;std::array<Node*,12> rows{};
    std::array<u32,12> groups{},length{};u32 height=0;
    explicit AffineRangeSum(std::span<const u32> values){
        u32 n=values.size()+1,size=0,width=1;
        for(;;){groups[height]=(n+15)/16;length[height]=width;if(height)size+=groups[height];++height;if(n<=16)break;n=(n+15)/16;width*=16;}
        sums=Buffer<u32>(16*groups[0]);std::fill(sums.p,sums.p+sums.n,0u);for(u32 i=0;i<values.size();++i)sums[i]=values[i];
        nodes=Buffer<Node>(size);for(auto& x:std::span(nodes.p,nodes.n)){std::fill(x.sums,x.sums+16,0u);std::fill(x.a,x.a+16,A::one);std::fill(x.b,x.b+16,0u);}
        for(u32 k=1,at=0;k<height;++k){rows[k]=nodes.p+at;at+=groups[k];for(u32 i=0;i<groups[k-1];++i)pull(k,i);}
    }
    u32* group(u32 k,u32 i){return k?rows[k][i].sums:sums.p+16*i;}
    const u32* group(u32 k,u32 i)const{return k?rows[k][i].sums:sums.p+16*i;}
    static u32 reduce(__m256i x) {
        // Four independent u64 sums avoid overflow of eight redundant residues.
        auto z=_mm256_setzero_si256();auto y=_mm256_add_epi64(_mm256_unpacklo_epi32(x,z),_mm256_unpackhi_epi32(x,z));
        y=_mm256_add_epi64(y,_mm256_permute2x128_si256(y,y,1));y=_mm256_add_epi64(y,_mm256_srli_si256(y,8));return u64(_mm256_extract_epi64(y,0))%P;
    }
    void pull(u32 k,u32 i){rows[k][i/16].sums[i&15]=reduce(_mm256_add_epi32(_mm256_load_si256((const __m256i*)(group(k-1,i))),_mm256_load_si256((const __m256i*)(group(k-1,i)+8))));}
    static __m256i mask(u32 l,u32 r){auto lanes=_mm256_setr_epi32(0,1,2,3,4,5,6,7);return _mm256_andnot_si256(_mm256_cmpgt_epi32(_mm256_set1_epi32(l),lanes),_mm256_cmpgt_epi32(_mm256_set1_epi32(r),lanes));}
    void block(u32 k,u32 i,u32 first,u32 last,Factor mult,u32 c){
        for(u32 j=first&-8u;j<last;j+=8){
        auto shift=_mm256_set1_epi32(c),selected=mask(first-j,last-j);
        auto* p=(__m256i*)(group(k,i/16)+j);auto old=_mm256_load_si256(p);auto next=A::add(mult(old),_mm256_set1_epi32(A::multiply(c,length[k])));
        _mm256_store_si256(p,_mm256_blendv_epi8(old,next,selected));
        if(k){auto* ap=(__m256i*)(rows[k][i/16].a+j);auto* bp=(__m256i*)(rows[k][i/16].b+j);auto x=_mm256_load_si256(ap),y=_mm256_load_si256(bp);
            _mm256_store_si256(ap,_mm256_blendv_epi8(x,mult(x),selected));_mm256_store_si256(bp,_mm256_blendv_epi8(y,A::add(mult(y),shift),selected));}
    }
    }
    void push(u32 k,u32 i){auto& node=rows[k][i/16];u32 at=i&15,m=node.a[at],c=node.b[at];if(m==A::one&&!c)return;block(k-1,16*i,0,16,Factor(A::decode(m)),c);node.a[at]=A::one;node.b[at]=0;}
    void prefetch(u32 l,u32 r)const{__builtin_prefetch(sums.p+(l&-16u),0,3);__builtin_prefetch(sums.p+(r&-16u),0,3);}
    void update_block(u32 k,u32 i,u32 first,u32 last,Factor mult,u32 m,u32 c){
        if(k||height==1){block(k,i,first,last,mult,c);return;}
        auto& node=rows[1][i/256];u32 at=i/16&15,old_a=node.a[at],old_b=node.b[at];if(old_a==A::one&&!old_b){block(k,i,first,last,mult,c);return;}
        // Apply the pending parent action and the new boundary action in one pass.
        u32 next_a=A::multiply(old_a,m),next_b=A::add(A::multiply(old_b,m),c);
        auto before=_mm256_set1_epi32(old_a),after=_mm256_set1_epi32(next_a);
        auto before_b=_mm256_set1_epi32(A::multiply(old_b,1)),after_b=_mm256_set1_epi32(A::multiply(next_b,1));
        for(u32 j=0;j<16;j+=8){auto selected=mask(first-j,last-j);auto* p=(__m256i*)(sums.p+i+j);auto scale=_mm256_blendv_epi8(before,after,selected),shift=_mm256_blendv_epi8(before_b,after_b,selected);
            _mm256_store_si256(p,A::add(A::multiply(_mm256_load_si256(p),scale),shift));}
        node.a[at]=A::one;node.b[at]=0;
    }
    void apply(u32 l,u32 r,Affine<P> f){
        if(l==r)return;u32 first=l,last=r;
        for(u32 k=height;--k>1;){u32 x=l>>(4*k),y=r>>(4*k);if((x<<(4*k))!=l)push(k,x);if((y<<(4*k))!=r&&((x<<(4*k))==l||x!=y))push(k,y);}
        Factor mult(f.a);u32 m=M::mont(f.a,M::r2),c=M::mont(f.b,M::r2);
        for(u32 k=0;l<r;++k){if(l/16==r/16){update_block(k,l&-16u,l&15,r&15,mult,m,c);break;}if(l&15)update_block(k,l&-16u,l&15,16,mult,m,c);if(r&15)update_block(k,r&-16u,0,r&15,mult,m,c);l=(l+15)/16;r/=16;}
        for(u32 k=1;k<height;++k){u32 bits=4*k,x=first>>bits,y=last>>bits;if((x<<bits)!=first)pull(k,x);if((y<<bits)!=last&&((x<<bits)==first||x!=y))pull(k,y);}
    }
    u32 fold(u32 k,u32 group,u32 l,u32 r)const{const auto* p=(const __m256i*)(this->group(k,group));return reduce(_mm256_add_epi32(_mm256_and_si256(_mm256_load_si256(p),mask(l,r)),_mm256_and_si256(_mm256_load_si256(p+1),mask(l-8,r-8))));}
    u32 lift(u32 k,u32 i,u32 sum,u32 count)const{
        if(!k||!count)return sum;const auto& node=rows[k][i/16];u32 at=i&15;
        // Sums/counts are ordinary, tags encoded: one mixed Montgomery reduction.
        u64 z=u64(sum)*node.a[at]+u64(node.b[at])*count;u32 value=(z+u64(u32(z)*M::inverse)*P)>>32;return std::min(value,value-2*P);
    }
    u32 sum(u32 l,u32 r)const{
        if(l==r)return 0;u32 first=l,last=r-1,lo=0,hi=0,nlo=0,nhi=0,width=1;
        for(u32 k=0;k<height;++k){u32 x=first/16,y=last/16;lo=lift(k,first,lo,nlo);hi=lift(k,last,hi,nhi);
            if(x==y){u32 value=A::add(lo,hi);if(l<r)value=A::add(value,fold(k,x,l-16*x,r-16*x));u32 count=nlo+nhi+(r-l)*width;
                for(++k;k<height;++k,x/=16)value=lift(k,x,value,count);return std::min(value,value-P);}
            if(l<r){if(l&15){lo=A::add(lo,fold(k,x,l&15,16));nlo=nlo+(16-(l&15))*width;}if(r&15){hi=A::add(hi,fold(k,y,0,r&15));nhi=nhi+(r&15)*width;}l=(l+15)/16;r/=16;}
            first=x;last=y;width*=16;
        }
        return 0;
    }
};
}
