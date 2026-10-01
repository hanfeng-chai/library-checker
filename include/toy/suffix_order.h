#pragma once
#include <toy/string_basic.h>
namespace toy {
namespace suffix_detail {
inline u32 periodic_order(std::string_view s,std::span<u32>sa){
    u32 n=s.size();if(n<256)return 0;std::array<u32,128>failure{};for(u32 i=1;i<128;++i){u32 j=failure[i-1];while(j&&s[i]!=s[j])j=failure[j-1];failure[i]=j+(s[i]==s[j]);}u32 period=128-failure[127];
    if(period>64||common_prefix(s.data(),s.data()+period,n-period)!=n-period)return 0;
    std::array<u32,127>entry;for(u32 i=0;i<period;++i)entry[i]=i;for(u32 i=0;i+1<period;++i)entry[period+i]=n-period+1+i;
    std::sort(entry.begin(),entry.begin()+2*period-1,[&](u32 a,u32 b){u32 na=std::min(period,n-a),nb=std::min(period,n-b),same=common_prefix(s.data()+a,s.data()+b,std::min(na,nb));return same<std::min(na,nb)?s[a+same]<s[b+same]:na<nb;});
    u32 at=0;for(u32 k=0;k<2*period-1;++k){u32 r=entry[k];if(r>=period)sa[at++]=r;else{u32 count=(n-period-r)/period+1;while(count)sa[at++]=r+(--count)*period;}}return period;
}
// Certify LMS order by a bounded prefix. A sample only decides whether to try;
// large buckets or unresolved equal keys fall back to ordinary SA-IS naming.
inline bool prefix_order(std::string_view s,std::span<u32>lms){
    u32 n=s.size(),m=lms.size();if(!m)return true;u32 low=127,high=0;for(u8 c:s){low=std::min(low,u32(c));high=std::max(high,u32(c));}u32 bits=std::bit_width(high-low);if(!bits)return false;
    u32 position_bits=std::bit_width(n),key_bits=(64-position_bits)/bits*bits,letters=key_bits/bits,shift=key_bits+position_bits-20;u64 mask=(1ull<<position_bits)-1;
    auto key=[&](u32 p){u64 value=0;for(u32 j=0;j<letters;++j)value=(value<<bits)|(p+j<n?u32(u8(s[p+j]))-low:0);return value;};
    std::array<u64,256>sample;u32 size=std::min(m,256u),duplicates=0;for(u32 i=0;i<size;++i)sample[i]=key(lms[u64(i)*m/size]);std::sort(sample.begin(),sample.begin()+size);for(u32 i=1;i<size;++i)duplicates+=sample[i]==sample[i-1];if(duplicates>size/16)return false;
    Buffer<u64>words(m),temp(m);
    // Eight letters use five bits each; append the ninth after BMI2 compression.
    if(bits==5&&key_bits==45){for(u32 i=0;i<m;++i){u32 p=lms[i];u64 value;
            if(p+9<=n){u64 bytes;memcpy(&bytes,s.data()+p,8);bytes=__builtin_bswap64(bytes-u64(low)*0x0101010101010101ull);value=(_pext_u64(bytes,0x1f1f1f1f1f1f1f1full)<<5)|(u8(s[p+8])-low);}else value=key(p);
            words[i]=(value<<position_bits)|(n-p);
        }
    }else{u64 value=0;u32 j=m;for(u32 p=n;p--;){value=(value>>bits)|(u64(u8(s[p])-low)<<(key_bits-bits));if(j&&p==lms[j-1])words[--j]=(value<<position_bits)|(n-p);}}
    u32 count[2][1024]{};for(u64 x:std::span(words.p,words.n)){++count[0][(x>>shift)&1023];++count[1][(x>>(shift+10))&1023];}
    for(auto&row:count){u32 sum=0;for(u32&x:row){u32 v=x;x=sum;sum+=v;}}
    for(u64 x:std::span(words.p,words.n))temp[count[0][(x>>shift)&1023]++]=x;for(u64 x:std::span(temp.p,temp.n))words[count[1][(x>>(shift+10))&1023]++]=x;
    for(u32 b=0;b<m;){u32 e=b+1;while(e<m&&(words[e]>>shift)==(words[b]>>shift))++e;if(e-b>64)return false;std::sort(words.p+b,words.p+e);b=e;}
    for(u32 i=1;i<m;++i)if((words[i]>>position_bits)==(words[i-1]>>position_bits)&&(words[i-1]&mask)>letters)return false;
    for(u32 i=0;i<m;++i)lms[i]=n-u32(words[i]&mask);return true;
}
// Complete LMS factors use all-one padding: a proper-prefix factor sorts
// after its extension (its endpoint is S-type). The terminal zero is unique.
template<class T>u32 factor_names(std::span<const T>s,std::span<const u32>lms,u32 upper,std::span<u32>names){
    u32 n=s.size(),m=lms.size();if(upper>=255||m<128)return 0;for(u32 i=0;i<m;++i)if((i+1<m?lms[i+1]:n)-lms[i]+1>8)return 0;
    std::array<u64,512>keys{};std::array<u16,512>ids{};std::array<u64,128>unique;u32 size=0;
    for(u32 i=0;i<m;++i){u32 p=lms[i],end=i+1<m?lms[i+1]:n,length=end-p+1;u64 key=0;for(u32 j=p;j<=end;++j)key=(key<<8)|(j<n?u32(s[j])+1:0);if(length<8)key=(key<<(64-8*length))|((1ull<<(64-8*length))-1);
        u32 slot=(key*0x9e3779b97f4a7c15ull)>>55,probes=0;while(ids[slot]&&keys[slot]!=key){if(++probes==8)return 0;slot=(slot+1)&511;}if(!ids[slot]){if(size==128)return 0;keys[slot]=key;unique[size]=key;ids[slot]=++size;}names[i]=ids[slot]-1;
    }
    std::array<u32,128>order,rank;std::iota(order.begin(),order.begin()+size,0u);std::sort(order.begin(),order.begin()+size,[&](u32 a,u32 b){return unique[a]<unique[b];});for(u32 i=0;i<size;++i)rank[order[i]]=i;for(u32&i:names)i=rank[i];return size;
}

}
}
