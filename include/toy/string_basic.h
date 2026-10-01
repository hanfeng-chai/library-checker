#pragma once
#include <toy/buffer.h>
namespace toy {
// Bound every vector load; callers need no sentinel or extra readable bytes.
inline u32 common_prefix(const char*a,const char*b,u32 limit){
    if(!limit||*a!=*b)return 0;u32 i=0;if(limit>=8){u64 x,y;memcpy(&x,a,8);memcpy(&y,b,8);if(x^y)return std::countr_zero(x^y)/8;i=8;}
    while(i+32<=limit){auto x=_mm256_loadu_si256((const __m256i*)(a+i)),y=_mm256_loadu_si256((const __m256i*)(b+i));u32 mask=~u32(_mm256_movemask_epi8(_mm256_cmpeq_epi8(x,y)));if(mask)return i+std::countr_zero(mask);i+=32;}
    for(;i+8<=limit;i+=8){u64 x,y;memcpy(&x,a+i,8);memcpy(&y,b+i,8);if(x^y)return i+std::countr_zero(x^y)/8;}
    for(;i<limit&&a[i]==b[i];++i);return i;
}
inline Buffer<u32> z_function(std::string_view s){
    u32 n=s.size();Buffer<u32>z(n);if(!n)return z;z[0]=n;u32 left=0,right=0;
    for(u32 i=1;i<n;++i){u32 k=i<right?std::min(right-i,z[i-left]):0;if(i+k<n&&s[k]==s[i+k])k+=common_prefix(s.data()+k,s.data()+i+k,n-i-k);z[i]=k;if(i+k>right){left=i;right=i+k;}}return z;
}
// Duval: returned boundaries include both zero and the full string length.
inline Buffer<u32> lyndon_factorization(std::string_view s){
    u32 n=s.size();Buffer<u32>boundary(1,n+1);boundary[0]=0;
    for(u32 i=0;i<n;){u32 j=i+1,k=i;while(j<n&&s[k]<=s[j]){if(s[k]<s[j]){k=i;++j;}else{u32 same=common_prefix(s.data()+k,s.data()+j,n-j);k+=same;j+=same;}}u32 period=j-k;do{i+=period;boundary.p[boundary.n++]=i;}while(i<=k);}return boundary;
}
// Lengths for alternating character / gap centers, without a separator copy.
inline Buffer<u32> palindrome_lengths(std::string_view s){
    u32 n=s.size();Buffer<u32>radius(2*n);if(!n)return radius;radius[0]=0;u32 center=0,right=0;
    for(u32 i=1;i<2*n;++i){u32 length=1;
        if(i<=right){u32 mirror=radius[2*center-i],limit=right-i;if(mirror<limit){radius[i]=mirror;continue;}length=limit;}
        while(length<i&&i+length<2*n&&s[(i-length-2)/2]==s[(i+length)/2])length+=2;
        radius[i]=length;if(i+length>right){center=i;right=i+length;}
    }
    memmove(radius.p,radius.p+1,(2*n-1)*4);radius.n=2*n-1;return radius;
}
}
