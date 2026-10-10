// https://judge.yosupo.jp/problem/matrix_rank_mod_2
#include <bits/stdc++.h>
#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,popcnt")
// Packed Gaussian elimination. Eight pivots share a table of XOR combinations.
struct Input {
    std::vector<char> data; char* p;
    Input(){struct stat s{};fstat(0,&s);if(S_ISREG(s.st_mode)&&s.st_size){p=(char*)mmap(nullptr,s.st_size,PROT_READ,MAP_PRIVATE,0,0);}else{char b[65536];size_t n;while((n=fread(b,1,sizeof b,stdin)))data.insert(data.end(),b,b+n);data.push_back(0);p=data.data();}}
    size_t number(){while(*p<=' ')p++;size_t n=0;while(*p>='0'&&*p<='9')n=10*n+(*p++-'0');return n;}
    char* row(size_t n){if(!n)return p;while(*p<=' ')p++;char* r=p;p+=n;return r;}
};
struct Bits {
    size_t n,m,w;std::vector<uint64_t> storage;std::vector<uint64_t*> a;std::vector<size_t> pivots;
    Bits(size_t n,size_t m):n(n),m(m),w((m+63)/64),storage(n*w),a(n){for(size_t i=0;i<n;i++)a[i]=storage.data()+i*w;}
    static bool get(uint64_t const* p,size_t j){return (p[j/64]>>(j%64))&1;}
    static void set(uint64_t* p,size_t j){p[j/64]|=uint64_t(1)<<(j%64);}
    void parse(size_t i,char const* s,size_t len){size_t j=0;for(;j+64<=len;j+=64){auto x=_mm256_loadu_si256((__m256i const*)(s+j)),y=_mm256_loadu_si256((__m256i const*)(s+j+32));a[i][j/64]=uint32_t(_mm256_movemask_epi8(_mm256_slli_epi64(x,7)))|(uint64_t(uint32_t(_mm256_movemask_epi8(_mm256_slli_epi64(y,7))))<<32);}for(;j<len;j++)if(s[j]=='1')set(a[i],j);}
    void xor_row(uint64_t* __restrict__ dst,uint64_t const* __restrict__ src,size_t first){
        size_t last=w,k=first;
        for(;k+4<=last;k+=4){auto x=_mm256_loadu_si256((__m256i const*)(dst+k)),y=_mm256_loadu_si256((__m256i const*)(src+k));_mm256_storeu_si256((__m256i*)(dst+k),_mm256_xor_si256(x,y));}
        for(;k<last;k++)dst[k]^=src[k];
    }
    size_t eliminate(size_t limit,bool reduced){
        std::vector<unsigned char> applied(n);size_t rank=0,col=0;std::vector<uint64_t> table((size_t(1)<<std::min({size_t(8),n,limit}))*w);
        while(rank<n && col<limit){
            std::fill(applied.begin()+rank,applied.end(),0);size_t start=rank,word=col/64;
            std::array<size_t,8> piv{};
            for(size_t t=0;t<8 && rank<n && col<limit;col++){
                size_t found=rank;
                while(found<n){
                    for(size_t k=applied[found];k<t;k++)if(get(a[found],piv[k]))xor_row(a[found],a[start+k],word);applied[found]=t;
                    if(get(a[found],col))break;
                    found++;
                }
                if(found==n){
                    size_t next=limit;
                    for(size_t i=rank;i<n;i++) {
                        size_t q=col/64;uint64_t bits=a[i][q]&(~uint64_t(0)<<(col%64));
                        while(!bits && (q+1)*64<next){q++;bits=a[i][q];}
                        if(bits)next=std::min(next,q*64+std::countr_zero(bits));
                    }
                    if(next==limit){col=limit;break;}
                    col=next-1;continue;
                }
                std::swap(a[rank],a[found]);std::swap(applied[rank],applied[found]);piv[t++]=col;pivots.push_back(col);rank++;
            }
            if(!reduced && rank==limit)return rank;size_t count=rank-start;if(!count)continue;
            for(size_t t=count;t-->0;)for(size_t k=0;k<t;k++)if(get(a[start+k],piv[t]))xor_row(a[start+k],a[start+t],word);
            // A table indexed by pivot bits now clears the entire panel at once.
            for(size_t mask=1;mask<(size_t(1)<<count);mask++){
                size_t k=std::countr_zero(mask),rest=mask&(mask-1);
                auto dst=table.data()+mask*w,src=table.data()+rest*w;
                auto pivot=a[start+k];size_t j=word,last=w;
                for(;j+4<=last;j+=4){auto x=_mm256_loadu_si256((__m256i const*)(src+j)),y=_mm256_loadu_si256((__m256i const*)(pivot+j));_mm256_storeu_si256((__m256i*)(dst+j),_mm256_xor_si256(x,y));}
                for(;j<last;j++)dst[j]=src[j]^pivot[j];
            }
            bool single_word=piv[0]/64==piv[count-1]/64;uint64_t select=0;for(size_t k=0;k<count;k++)select|=uint64_t(1)<<(piv[k]%64);
            for(size_t i=reduced?0:rank;i<n;i++){
                if(i>=start&&i<rank)continue;
                unsigned mask=0;
                if(single_word)mask=_pext_u64(a[i][piv[0]/64],select);
                else for(size_t k=0;k<count;k++)mask|=unsigned(get(a[i],piv[k]))<<k;
                if(mask)xor_row(a[i],table.data()+mask*w,word);
            }
        }
        return rank;
    }
};
void print_bits(uint64_t const* a,size_t begin,size_t len){
    std::string s(len,'0');size_t j=0;
    for(;j+32<=len;j+=32){size_t pos=begin+j;uint64_t v=a[pos/64]>>(pos%64);if(pos%64>32)v|=a[pos/64+1]<<(64-pos%64);
        auto bytes=_mm256_shuffle_epi8(_mm256_set1_epi32(uint32_t(v)),_mm256_setr_epi64x(0,0x0101010101010101LL,0x0202020202020202LL,0x0303030303030303LL));
        auto mask=_mm256_set1_epi64x(0x8040201008040201ULL);auto bits=_mm256_cmpeq_epi8(_mm256_and_si256(bytes,mask),mask);
        _mm256_storeu_si256((__m256i*)(s.data()+j),_mm256_sub_epi8(_mm256_set1_epi8('0'),bits));}
    for(;j<len;j++)s[j]+=(a[(begin+j)/64]>>((begin+j)%64))&1;
    fwrite(s.data(),1,s.size(),stdout);putchar('\n');
}

int main(){Input in;size_t n=in.number(),m=in.number();if(!n||!m){puts("0");return 0;}
    if(m<=64){uint64_t basis[64]{};size_t r=0;for(size_t i=0;i<n;i++){auto s=in.row(m);uint64_t v=0;for(size_t j=0;j<m;j++)v|=uint64_t(s[j]=='1')<<j;while(v){size_t j=std::countr_zero(v);if(basis[j])v^=basis[j];else{basis[j]=v;r++;break;}}if(r==m)break;}printf("%zu\n",r);return 0;}

    if(n>2*m){
        Bits a(m,n);
        for(size_t row=0;row<n;row+=64){
            char* src[64];size_t nr=std::min(size_t(64),n-row);for(size_t i=0;i<nr;i++)src[i]=in.row(m);
            for(size_t col=0;col<m;col+=64){
                uint64_t tile[64]{};size_t nc=std::min(size_t(64),m-col);
                for(size_t i=0;i<nr;i++){
                    size_t j=0;uint64_t v=0;
                    for(;j+32<=nc;j+=32){auto x=_mm256_loadu_si256((__m256i const*)(src[i]+col+j));v|=uint64_t(uint32_t(_mm256_movemask_epi8(_mm256_slli_epi64(x,7))))<<j;}
                    for(;j<nc;j++)v|=uint64_t(src[i][col+j]=='1')<<j;
                    tile[i]=v;
                }
                uint64_t mask=0x00000000FFFFFFFFULL;
                for(size_t shift=32;shift;shift>>=1,mask^=mask<<shift){
                    for(size_t k=0;k<64;k=(k+shift+1)&~shift){uint64_t t=((tile[k]>>shift)^tile[k+shift])&mask;tile[k]^=t<<shift;tile[k+shift]^=t;}
                }
                for(size_t j=0;j<nc;j++)a.a[col+j][row/64]=tile[j];
            }
        }
        printf("%zu\n",a.eliminate(n,false));return 0;
    }
    Bits a(n,m);for(size_t i=0;i<n;i++)a.parse(i,in.row(m),m);printf("%zu\n",a.eliminate(m,false));}
