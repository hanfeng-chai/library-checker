#pragma once
#include <toy/suffix_array.h>
#include <toy/rmq.h>
#include <toy/radix_sort.h>
namespace toy {
struct SuffixLCE {
    u32 n;Buffer<u32>rank;RMQ<u32>minimum;
    explicit SuffixLCE(std::string_view s):n(s.size()),rank(n),minimum([&]{auto sa=suffix_array(s);for(u32 i=0;i<n;++i)rank[sa[i]]=i;return lcp_array(s,sa);}()){}
    u32 query(u32 a,u32 b)const{if(a==n||b==n)return 0;if(a==b)return n-a;a=rank[a];b=rank[b];if(a>b)std::swap(a,b);return minimum.min(a,b);}
};
struct AdaptiveLCE {
    std::string_view text;u32 budget;std::optional<SuffixLCE>index;
    explicit AdaptiveLCE(std::string_view s):text(s),budget(64*s.size()){}
    // Keep index construction out of the hot, inlined bounded comparison.
    [[gnu::noinline]] u32 indexed(u32 a,u32 b,u32 limit){if(!index)index.emplace(text);return std::min(limit,index->query(a,b));}
    // Both ranges of length limit must be within text. Only matched bytes
    // consume budget; the caller already makes O(n) comparisons.
    [[gnu::always_inline]] u32 query(u32 a,u32 b,u32 limit){
        if(!budget)return indexed(a,b,limit);u32 length=std::min(limit,budget),same=common_prefix(text.data()+a,text.data()+b,length);budget-=same;
        if(same==length&&length<limit)return indexed(a,b,limit);return same;
    }
};
// n < 2^21, so each run fits in a 63-bit key.
// Two opposite alphabet orders enumerate Lyndon roots. Exact forward/backward
// LCE extends each root to its maximal periodic interval; no hashing is used.
inline Buffer<std::array<u32,3>> string_runs(std::string_view s){
    u32 n=s.size();Buffer<u64>runs(0,2*n);if(n<2)return {};if(common_prefix(s.data(),s.data()+1,n-1)==n-1){Buffer<std::array<u32,3>>answer(1);answer[0]={1,0,n};return answer;}u32 bits=std::bit_width(n);AdaptiveLCE forward(s);Buffer<char>reverse(n);std::reverse_copy(s.begin(),s.end(),reverse.p);AdaptiveLCE backward(std::string_view(reverse.p,n));Buffer<u32>stack(n+1);
    for(bool inverted:{false,true}){u32 top=0;stack[0]=n;
        for(u32 i=n;i--;){u32 common=0;while(top){u32 j=stack[top],k=stack[top-1],bound=std::min(j-i,k-j);common=s[i]==s[j]?forward.query(i,j,bound):0;
                if(common==bound?j-i>=k-j:((s[i+common]>=s[j+common])^inverted))break;--top;common=0;
            }
            u32 j=stack[top],period=j-i;stack[++top]=i;if(j==n)continue;u32 left=i&&s[i-1]==s[j-1]?backward.query(n-i,n-j,std::min(i,period)):0;
            if(left<period){u32 right=common;if(j+common<n&&s[i+common]==s[j+common])right+=forward.query(i+common,j+common,n-j-common);if(left+right>=period)runs.p[runs.n++]=(u64(period)<<(2*bits))|(u64(i-left)<<bits)|(j+right);}
        }
    }
    if(bits<=18)radix_sort<54,11>(std::span(runs.p,runs.n),[](u64 x){return x;});else radix_sort<63,11>(std::span(runs.p,runs.n),[](u64 x){return x;});runs.n=std::unique(runs.p,runs.p+runs.n)-runs.p;
    Buffer<std::array<u32,3>>answer(runs.n);u32 mask=(1u<<bits)-1;for(u32 i=0;i<runs.n;++i){u64 x=runs[i];answer[i]={u32(x>>(2*bits)),u32(x>>bits)&mask,u32(x)&mask};}return answer;
}
}
