#pragma once
#include <toy/bit_matrix.h>
namespace toy {
// Classical packed Gaussian elimination, one pivot per pass.
inline u32 bit_gaussian(BitMatrix&a,u32 columns,bool reduced=false){u32 rank=0;for(u32 c=0;c<columns&&rank<a.n;++c){u32 p=rank;while(p<a.n&&!a.get(p,c))++p;if(p==a.n)continue;std::swap(a.row[p],a.row[rank]);a.pivot.p[rank]=c;for(u32 i=reduced?0:rank+1;i<a.n;++i)if(i!=rank&&a.get(i,c))xor_words(a[i],a[rank],c/64,a.stride);++rank;}a.pivot.n=rank;return rank;}
}
