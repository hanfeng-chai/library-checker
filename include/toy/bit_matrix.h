#pragma once
#include <toy/buffer.h>
namespace toy {
inline void xor_words(u64* __restrict a,const u64* __restrict b,u32 first,u32 end){u32 j=first;for(;j+4<=end;j+=4)_mm256_storeu_si256((__m256i*)(a+j),_mm256_xor_si256(_mm256_loadu_si256((const __m256i*)(a+j)),_mm256_loadu_si256((const __m256i*)(b+j))));for(;j<end;++j)a[j]^=b[j];}
// ASCII comparisons pack 32 columns; the first character is the low bit.
inline void pack_bits(u64*out,std::string_view s){u32 j=0;for(;j+32<=s.size();j+=32)out[j/64]|=u64(u32(_mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_loadu_si256((const __m256i*)(s.data()+j)),_mm256_set1_epi8('1')))))<<(j%64);for(;j<s.size();++j)out[j/64]|=u64(s[j]=='1')<<(j%64);}
// PDEP puts one bit in each byte; OR with ASCII '0' emits eight characters.
inline void unpack_bits(const u64*in,u32 first,std::span<char>out){u32 j=0;for(;j+8<=out.size();j+=8){u32 p=first+j;u64 x=in[p/64]>>(p%64);if(p%64>56)x|=in[p/64+1]<<(64-p%64);x=_pdep_u64(x,0x0101010101010101ull)|0x3030303030303030ull;memcpy(out.data()+j,&x,8);}for(;j<out.size();++j)out[j]='0'+((in[(first+j)/64]>>((first+j)%64))&1);}
struct BitMatrix {
    u32 n,m,stride;Buffer<u64>data;Buffer<u64*>row;Buffer<u32>pivot;
    BitMatrix(u32 n,u32 m):n(n),m(m),stride((m+63)/64),data(usize(n)*stride),row(n),pivot(0,std::min(n,m)){std::fill(data.p,data.p+data.n,0ull);for(u32 i=0;i<n;++i)row[i]=data.p+usize(i)*stride;}
    u64*operator[](u32 i){return row[i];}const u64*operator[](u32 i)const{return row[i];}
    bool get(u32 i,u32 j)const{return (row[i][j/64]>>(j%64))&1;}
    void set(u32 i,u32 j){row[i][j/64]|=1ull<<(j%64);}
    // Panel pivots are reduced to an identity block. One table lookup then
    // eliminates all panel columns together (the Four Russians method).
    template<u32 Block=8>u32 eliminate(u32 columns,bool reduced=false){Buffer<u64>table(usize(1u<<Block)*stride);u32 rank=0,words=(columns+63)/64;
        while(rank<n&&rank<columns){u32 size=0;
            for(u32 i=rank;i<n&&size<Block;++i){for(u32 j=0;j<size;++j)if(get(i,pivot[rank+j]))xor_words(row[i],row[rank+j],0,stride);
                u32 column=columns;for(u32 w=0;w<words;++w){u64 x=row[i][w];if(w+1==words&&columns%64)x&=(1ull<<(columns%64))-1;if(x){column=64*w+std::countr_zero(x);break;}}
                if(column<columns){std::swap(row[rank+size],row[i]);pivot.p[rank+size++]=column;}
            }
            if(!size)break;
            for(u32 j=size;j--;)for(u32 i=0;i<j;++i)if(get(rank+i,pivot[rank+j]))xor_words(row[rank+i],row[rank+j],0,stride);
            for(u32 i=1;i<size;++i)for(u32 j=i;j&&pivot[rank+j]<pivot[rank+j-1];--j){std::swap(pivot[rank+j],pivot[rank+j-1]);std::swap(row[rank+j],row[rank+j-1]);}
            u32 first=pivot[rank]/64,last=pivot[rank+size-1]/64;u64 mask0=0,mask1=0;for(u32 i=0;i<size;++i){u32 p=pivot[rank+i];if(p/64==first)mask0|=1ull<<(p%64);else if(p/64==first+1)mask1|=1ull<<(p%64);}
            u32 finish=first+1;for(u32 q=0;q<size&&finish<stride;++q){u32 w=stride;while(w>finish&&!row[rank+q][w-1])--w;finish=std::max(finish,w);}
            std::fill(table.p+first,table.p+finish,0ull);
            for(u32 bits=1;bits<(1u<<size);++bits){u32 bit=std::countr_zero(bits);u64*dst=table.p+usize(bits)*stride;const u64*src=table.p+usize(bits&(bits-1))*stride,*add=row[rank+bit];u32 j=first;for(;j+4<=finish;j+=4)_mm256_storeu_si256((__m256i*)(dst+j),_mm256_xor_si256(_mm256_loadu_si256((const __m256i*)(src+j)),_mm256_loadu_si256((const __m256i*)(add+j))));for(;j<finish;++j)dst[j]=src[j]^add[j];}
            for(u32 i=reduced?0:rank+size;i<n;++i){if(i>=rank&&i<rank+size)continue;u32 key;
                if(last<=first+1){key=_pext_u64(row[i][first],mask0);if(mask1)key|=_pext_u64(row[i][first+1],mask1)<<std::popcount(mask0);}
                else{key=0;for(u32 j=0;j<size;++j)key|=u32(get(i,pivot[rank+j]))<<j;}
                if(key)xor_words(row[i],table.p+usize(key)*stride,first,finish);
            }
            rank+=size;pivot.n=rank;
        }return rank;
    }
};
inline BitMatrix bit_matrix_product(const BitMatrix&a,const BitMatrix&b){BitMatrix c(a.n,b.m);Buffer<u64>table(256*b.stride);std::fill(table.p,table.p+b.stride,0ull);
    for(u32 k=0;k<a.m;k+=8){u32 bits=std::min(8u,a.m-k);for(u32 mask=1;mask<(1u<<bits);++mask){u32 bit=std::countr_zero(mask);u64*dst=table.p+usize(mask)*b.stride;const u64*src=table.p+usize(mask&(mask-1))*b.stride,*add=b[k+bit];u32 j=0;for(;j+4<=b.stride;j+=4)_mm256_storeu_si256((__m256i*)(dst+j),_mm256_xor_si256(_mm256_loadu_si256((const __m256i*)(src+j)),_mm256_loadu_si256((const __m256i*)(add+j))));for(;j<b.stride;++j)dst[j]=src[j]^add[j];}
        for(u32 i=0;i<a.n;++i){u32 key=(a[i][k/64]>>(k%64))&((1u<<bits)-1);if(key)xor_words(c[i],table.p+usize(key)*b.stride,0,b.stride);}
    }return c;
}
// Inputs are independent bases in F_2^Bits. A full space or a hyperplane has
// a cheaper certificate; otherwise upper bits retain the U component.
template<u32 Bits=32>Buffer<u32> binary_intersection(std::span<const u32>u,std::span<const u32>v){
    if(u.size()==Bits||v.empty()){Buffer<u32>r(v.size());if(r.n)memcpy(r.p,v.data(),v.size_bytes());return r;}
    if(v.size()==Bits||u.empty()){Buffer<u32>r(u.size());if(r.n)memcpy(r.p,u.data(),u.size_bytes());return r;}
    Buffer<u32>answer(0,Bits);
    if(u.size()==Bits-1||v.size()==Bits-1){if(v.size()==Bits-1)std::swap(u,v);std::array<u32,Bits>basis{};for(u32 x:u)while(x){u32 bit=std::bit_width(x)-1;if(!basis[bit]){basis[bit]=x;break;}x^=basis[bit];}u32 free=0;while(basis[free])++free;u32 normal=1u<<free;for(u32 i=0;i<Bits;++i)if(std::popcount(basis[i]&normal)&1)normal^=1u<<i;
        u32 odd=0;for(u32 x:v){if(std::popcount(x&normal)&1){if(!odd){odd=x;continue;}x^=odd;}answer.p[answer.n++]=x;}return answer;
    }
    std::array<u64,Bits>basis{};auto insert=[&](u64 x){while(u32(x)){u32 bit=std::bit_width(u32(x))-1;if(!basis[bit]){basis[bit]=x;return u32(0);}x^=basis[bit];}return u32(x>>32);};for(u32 x:u)insert((u64(x)<<32)|x);for(u32 x:v)if(u32 y=insert(x))answer.p[answer.n++]=y;return answer;
}
}
