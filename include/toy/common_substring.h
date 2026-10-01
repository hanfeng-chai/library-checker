#pragma once
#include <toy/string_basic.h>
#include <toy/suffix_automaton.h>
#include <toy/page_buffer.h>
namespace toy {
inline u32 common_suffix(const char*a,const char*b,u32 limit){u32 i=0;while(i+32<=limit){auto x=_mm256_loadu_si256((const __m256i*)(a-i-32)),y=_mm256_loadu_si256((const __m256i*)(b-i-32));u32 mask=~u32(_mm256_movemask_epi8(_mm256_cmpeq_epi8(x,y)));if(mask)return i+std::countl_zero(mask);i+=32;}while(i<limit&&a[-i32(i)-1]==b[-i32(i)-1])++i;return i;}
// Packed factors are exact keys. Hashing only chooses a slot; all occurrences
// are retained and compared. Work limits return nullopt for an exact fallback.
inline std::optional<std::array<u32,4>> seeded_substring(std::string_view s,std::string_view t,u32 low,u32 high,std::array<u32,4>best){
    u32 n=s.size(),m=t.size();if(n<256||low==high)return {};u32 bits=std::bit_width(high-low),q=1;u64 domain=high-low+1;while(domain<8ull*n&&(q+1)*bits<=32){++q;domain*=high-low+1;}
    u32 mask=(1ull<<(q*bits))-1,positions=n-q+1;auto key_at=[&](const char*p){u32 key=0;for(u32 i=0;i<q;++i)key=(key<<bits)|(u8(p[i])-low);return key;};
    std::array<u32,128>sample;for(u32 i=0;i<128;++i)sample[i]=key_at(s.data()+u64(i)*(positions-1)/127);std::sort(sample.begin(),sample.end());u32 duplicates=0;for(u32 i=1;i<128;++i)duplicates+=sample[i]==sample[i-1];if(duplicates>16)return {};
    u32 capacity=std::bit_ceil(2*positions),shift=32-std::countr_zero(capacity);auto table=page_buffer<u64>(capacity);std::fill(table.p,table.p+capacity,0ull);Buffer<u32>next(positions);u64 budget=8ull*(n+m);
    auto slot=[&](u32 key){u32 i=(key*0x9e3779b9u)>>shift;while(table[i]&&u32(table[i]>>32)!=key){if(!budget--)return capacity;i=(i+1)&(capacity-1);}return i;};
    u32 key=key_at(s.data());for(u32 i=0;i<positions;++i){u32 at=slot(key);if(at==capacity)return {};next[i]=u32(table[at]);table[at]=(u64(key)<<32)|(i+1);if(i+1<positions)key=((key<<bits)|(u8(s[i+q])-low))&mask;}
    u64 work=0,limit=4ull*(n+m);u32 length=best[1]-best[0];
    for(u32 j=0;j+q<=m;){u32 at=slot(key_at(t.data()+j));if(at==capacity)return {};
        for(u32 link=u32(table[at]);link;link=next[link-1]){u32 i=link-1;if(++work>limit)return {};u32 left_limit=std::min(i,j),right_limit=std::min(n-i,m-j);if(left_limit+right_limit<=length)continue;
            u32 right=q+common_prefix(s.data()+i+q,t.data()+j+q,right_limit-q),left=common_suffix(s.data()+i,t.data()+j,left_limit);work+=left+right;if(left+right>length){length=left+right;best={i-left,i+right,j-left,j+right};}if(length==n)return best;if(work>limit)return {};
        }
        // Any longer match contains a complete sampled q-letter factor.
        j+=length>=q?length-q+2:1;
    }
    if(length+1<q)return {};return best;
}
inline std::array<u32,4> common_substring(std::string_view s,std::string_view t){
    if(s.size()>t.size()){auto r=common_substring(t,s);return {r[2],r[3],r[0],r[1]};}u32 n=s.size(),m=t.size(),same=common_prefix(s.data(),t.data(),n);std::array<u32,4>best{0,same,0,same};if(same==n)return best;
    if(n<m)if(const void*found=memmem(t.data(),m,s.data(),n)){u32 p=(const char*)found-t.data();return {0,n,p,p+n};}
    u32 a=0,b=0;for(char c:s)a|=1u<<(c-'a');for(char c:t)b|=1u<<(c-'a');if(!(a&b))return {};
    if(std::has_single_bit(a)||std::has_single_bit(b)){bool flipped=std::has_single_bit(b)&&!std::has_single_bit(a);auto text=flipped?s:t;u32 cap=flipped?m:n,length=0,maximum=0,end=0;char c='a'+std::countr_zero(flipped?b:a);for(u32 i=0;i<text.size();++i){length=text[i]==c?length+1:0;if(length>maximum){maximum=std::min(length,cap);end=i+1;if(maximum==cap)break;}}return flipped?std::array<u32,4>{end-maximum,end,0,maximum}:std::array<u32,4>{0,maximum,end-maximum,end};}
    u32 alphabet=a|b,low='a'+std::countr_zero(alphabet),high='a'+31-std::countl_zero(alphabet);
    if(auto r=seeded_substring(s,t,low,high,best))return *r;
    if(2*u32(std::popcount(alphabet))<high-low+1){Buffer<char>text(n+m);for(u32 i=0;i<n;++i)text[i]=std::popcount(alphabet&((1u<<(s[i]-'a'))-1));for(u32 i=0;i<m;++i)text[n+i]=std::popcount(alphabet&((1u<<(t[i]-'a'))-1));if(auto r=seeded_substring({text.p,n},{text.p+n,m},0,std::popcount(alphabet)-1,best))return *r;}
    return SuffixAutomaton(s).longest_common_substring(t);
}
}
