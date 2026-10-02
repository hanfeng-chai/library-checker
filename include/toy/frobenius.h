#pragma once
#include <toy/matrix_product.h>
#include <toy/matrix_arbitrary.h>
#include <toy/polynomial_mod.h>
namespace toy {
template<bool Full=false,u32 P=998244353>struct Frobenius {
    using M=Mod<P>;u32 n,stride,width,rank=0,blocks=0;u64 random=0x183947ac92d78165ull;
    Buffer<u32>a,basis,raw,inverse,pivot,pivot_inverse,reductions,polynomials,offset;
    Frobenius(u32 n,std::span<const u32>input):n(n),stride((n+15)&~15u),width(Full?((2*n+8)&~7u):stride),a(page_buffer<u32>(usize(stride)*stride)),basis(usize(n)*width),raw(Full?usize(n)*stride:0),inverse(Full?usize(n)*n:0),pivot(n),pivot_inverse(n),reductions(Full?0:usize(n)*n),polynomials(0,2*n+1),offset(n+1){
        std::fill(a.p,a.p+a.n,0u);for(u32 i=0;i<n;++i)memcpy(a.p+usize(i)*stride,input.data()+usize(i)*n,n*4);auto scale=_mm256_set1_epi32(M::r2);for(usize i=0;i<a.n;i+=8)_mm256_storeu_si256((__m256i*)(a.p+i),M::mont(_mm256_loadu_si256((const __m256i*)(a.p+i)),scale));
        for(;;){rank=blocks=polynomials.n=0;offset[0]=0;bool retry=false;
            while(rank<n){u32 start=rank;Buffer<u32>x(stride);std::fill(x.p,x.p+stride,0u);for(u32 j=0;j<n;++j){random^=random<<13;random^=random>>7;random^=random<<17;x[j]=random%P;}for(u32 i=0;i<rank;++i)x[pivot[i]]=0;
                if(std::all_of(x.p,x.p+n,[](u32 v){return !v;}))for(u32 j=0;j<n;++j)if(std::find(pivot.p,pivot.p+rank,j)==pivot.p+rank){x[j]=1;break;}
                auto recurrence=generate(std::move(x),start);
                if constexpr(Full)if(std::any_of(recurrence.p,recurrence.p+start,[](u32 v){return v!=0;})){
                    u32 degree=rank-start;Buffer<u32>quotient(start+1),remainder(recurrence.n);memcpy(remainder.p,recurrence.p,recurrence.n*4);const u32*g=recurrence.p+start;
                    for(u32 i=rank+1;i-->degree;){u32 value=remainder[i];quotient[i-degree]=value;for(u32 j=0;j<degree;++j)remainder[i-degree+j]=M::sub(remainder[i-degree+j],M::mul(value,g[j]));}
                    Buffer<u32>adjusted(stride);memcpy(adjusted.p,raw.p+usize(start)*stride,stride*4);for(u32 j=0;j<start;++j)if(quotient[j])modular_row_sub(adjusted.p,raw.p+usize(j)*stride,0,n,P-quotient[j],P);
                    rank=start;recurrence=generate(std::move(adjusted),start);
                    // A successful block has no component in earlier chains;
                    // reject the random decomposition rather than an inexact result.
                    if(std::any_of(recurrence.p,recurrence.p+start,[](u32 v){return v!=0;})){retry=true;break;}
                }
                u32 first=Full?start:0,length=recurrence.n-first;memcpy(polynomials.p+polynomials.n,recurrence.p+first,length*4);polynomials.n+=length;offset[++blocks]=polynomials.n;
            }
            if(!retry)break;
        }
        if constexpr(Full){for(u32 j=n;j--;)for(u32 i=0;i<j;++i)if(u32 value=row(i)[pivot[j]]){u32 factor=M::mul(value,pivot_inverse[j]);modular_row_sub(row(i),row(j),n,2*n,factor,P);row(i)[pivot[j]]=0;}
            for(u32 i=0;i<n;++i)for(u32 j=0;j<n;++j)inverse[usize(pivot[i])*n+j]=M::mul(row(i)[n+j],pivot_inverse[i]);
        }
    }
    u32*row(u32 i){return basis.p+usize(i)*width;}
    // Row-vector times matrix. Padded zeros let the hot loop use eight terms;
    // only A is Montgomery encoded, so the output is canonical again.
    [[gnu::noinline]] void apply(const u32*x,u32*out)const{auto limit=_mm256_set1_epi64x(u64(2*P)<<32);
        for(u32 j=0;j<stride;j+=8){auto even=_mm256_setzero_si256(),odd=even;for(u32 first=0;first<stride;first+=8){for(u32 i=first;i<first+8;++i){auto value=_mm256_loadu_si256((const __m256i*)(a.p+usize(i)*stride+j)),scale=_mm256_set1_epi32(x[i]);even=_mm256_add_epi64(even,_mm256_mul_epu32(value,scale));odd=_mm256_add_epi64(odd,_mm256_mul_epu32(_mm256_srli_epi64(value,32),scale));}even=_mm256_min_epu32(even,_mm256_sub_epi32(even,limit));odd=_mm256_min_epu32(odd,_mm256_sub_epi32(odd,limit));}_mm256_storeu_si256((__m256i*)(out+j),M::mont_sum8(even,odd));}
    }
    Buffer<u32> generate(Buffer<u32>x,u32 start){Buffer<u32>y(width),next(stride),relation(n+1);
        for(;;){memcpy(y.p,x.p,n*4);std::fill(y.p+n,y.p+width,0u);if constexpr(Full)y[n+rank]=1;
            for(u32 i=0;i<rank;++i){u32 factor=M::mul(y[pivot[i]],pivot_inverse[i]);if constexpr(!Full)if(i>=start)relation[i-start]=M::sub(0,factor);if(factor)modular_row_sub(y.p,row(i),pivot[i],Full?n+i+1:n,factor,P);}
            u32 p=0;while(p<n&&!y[p])++p;
            if(p==n){if constexpr(Full){relation.n=rank+1;memcpy(relation.p,y.p+n,relation.n*4);}else{relation.n=rank-start+1;relation[rank-start]=1;for(u32 i=rank;i-->start;)for(u32 j=start;j<i;++j)relation[j-start]=M::add(relation[j-start],M::mul(relation[i-start],reductions[usize(i)*n+j]));}return relation;}
            pivot[rank]=p;pivot_inverse[rank]=M::pow(y[p],P-2);memcpy(row(rank),y.p,width*4);if constexpr(Full)memcpy(raw.p+usize(rank)*stride,x.p,stride*4);else for(u32 i=start;i<rank;++i)reductions[usize(rank)*n+i]=relation[i-start];++rank;apply(x.p,next.p);std::swap(x,next);
        }
    }
    std::span<const u32> polynomial(u32 block)const{return {polynomials.p+offset[block],offset[block+1]-offset[block]};}
};
template<u32 P=998244353>Buffer<u32> characteristic_polynomial(u32 n,std::span<const u32>a){using M=Mod<P>;bool upper=true,lower=true;for(u32 i=0;i<n&&(upper||lower);++i)for(u32 j=0;j<i;++j){upper&=a[usize(i)*n+j]==0;lower&=a[usize(j)*n+i]==0;}Buffer<u32>answer(1);answer[0]=1;
    if(upper||lower){for(u32 i=0;i<n;++i){answer.resize(i+2);for(u32 j=i+2;--j;)answer[j]=M::sub(answer[j-1],M::mul(a[usize(i)*n+i],answer[j]));answer[0]=M::sub(0,M::mul(a[usize(i)*n+i],answer[0]));}return answer;}
    Frobenius<false,P>form(n,a);for(u32 b=0;b<form.blocks;++b){auto p=form.polynomial(b);Buffer<u32>factor(p.size());memcpy(factor.p,p.data(),p.size_bytes());answer=convolution<P>(std::move(answer),std::move(factor));}return answer;
}
template<u32 P=998244353>Buffer<u32> polynomial_x_power(u64 exponent,std::span<const u32>modulus){using M=Mod<P>;u32 degree=modulus.size()-1;Buffer<u32>r(1);r[0]=1;if(degree==1){r[0]=M::pow(M::sub(0,modulus[0]),exponent);return r;}PolynomialMod<P>ring(modulus);
    for(u32 bit=std::bit_width(exponent);bit--;){r=ring.square(std::move(r));if((exponent>>bit)&1){r.resize(r.n+1);for(u32 i=r.n;--i;)r[i]=r[i-1];r[0]=0;if(r.n>degree){u32 leading=r[degree];for(u32 i=0;i<degree;++i)r[i]=M::sub(r[i],M::mul(leading,modulus[i]));r.n=degree;}while(r.n&&!r[r.n-1])--r.n;}}return r;
}
template<u32 P=998244353>Buffer<u32> matrix_power(u32 n,std::span<const u32>a,u64 exponent){using M=Mod<P>;if(exponent<=2){if(exponent==2)return matrix_product<P>(n,n,n,a,a);Buffer<u32>r(usize(n)*n);if(exponent)memcpy(r.p,a.data(),a.size_bytes());else{std::fill(r.p,r.p+r.n,0u);for(u32 i=0;i<n;++i)r[usize(i)*n+i]=1;}return r;}
    Buffer<u32>next(n),weight(n);bool monomial=true;for(u32 i=0;i<n&&monomial;++i){next[i]=weight[i]=0;for(u32 j=0;j<n;++j)if(u32 value=a[usize(i)*n+j]){if(weight[i]){monomial=false;break;}next[i]=j;weight[i]=value;}}
    if(monomial){Buffer<u32>to(n),value(n),old(n),old_weight(n);std::iota(to.p,to.p+n,0u);std::fill(value.p,value.p+n,1u);for(;exponent;exponent>>=1){if(exponent&1)for(u32 i=0;i<n;++i){value[i]=M::mul(value[i],weight[to[i]]);to[i]=next[to[i]];}memcpy(old.p,next.p,n*4);memcpy(old_weight.p,weight.p,n*4);for(u32 i=0;i<n;++i){next[i]=old[old[i]];weight[i]=M::mul(old_weight[i],old_weight[old[i]]);}}Buffer<u32>r(usize(n)*n);std::fill(r.p,r.p+r.n,0u);for(u32 i=0;i<n;++i)r[usize(i)*n+to[i]]=value[i];return r;}
    Frobenius<true,P>form(n,a);Buffer<u32>transformed(usize(n)*n);u32 start=0;
    for(u32 block=0;block<form.blocks;++block){auto polynomial=form.polynomial(block);u32 d=polynomial.size()-1;auto x=polynomial_x_power<P>(exponent,polynomial);x.resize(d);Buffer<u32>coeff(usize(d)*d),rows(usize(d)*n);
        for(u32 i=0;i<d;++i){memcpy(coeff.p+usize(i)*d,x.p,d*4);memcpy(rows.p+usize(i)*n,form.raw.p+usize(start+i)*form.stride,n*4);u32 last=x[d-1];for(u32 j=d;j--;)x[j]=M::sub(j?x[j-1]:0,M::mul(last,polynomial[j]));}
        auto result=matrix_product<P>(d,d,n,std::span(coeff.p,coeff.n),std::span(rows.p,rows.n));memcpy(transformed.p+usize(start)*n,result.p,result.n*4);start+=d;
    }return matrix_product<P>(n,n,n,std::span(form.inverse.p,form.inverse.n),std::span(transformed.p,transformed.n));
}
}
