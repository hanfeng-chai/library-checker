#pragma once
#include <toy/mod.h>
#include <toy/buffer.h>
namespace toy {
// Polynomial pair elimination with inclusion-exclusion. Level d retains only
// degrees <= m-d, so depth-first evaluation needs O(m^4) temporary storage.
template<u32 P=998244353>u32 hafnian(u32 n,std::span<const u32>a){using M=Mod<P>;if(!n)return 1;u32 m=n/2;bool constant=true;for(u32 i=1;i<n&&constant;++i)for(u32 j=0;j<i;++j)if(a[i*n+j]!=a[n]){constant=false;break;}if(constant){u32 result=M::pow(a[n],m);for(u32 i=1;i<n;i+=2)result=M::mul(result,i);return result;}
    Buffer<u32>offset(m+2);offset[0]=offset[1]=0;for(u32 d=1;d<=m;++d)offset[d+1]=offset[d]+d*(2*d-1)*(m-d+1);Buffer<u32>data(offset[m+1]),work(m*(m+1));std::fill(data.p,data.p+data.n,0u);auto at=[&](u32 d,u32 i,u32 j){return data.p+offset[d]+(i*(i-1)/2+j)*(m-d+1);};for(u32 i=1;i<n;++i)for(u32 j=0;j<i;++j)at(m,i,j)[0]=a[i*n+j];
    auto solve=[&](auto&&self,u32 d,u32*res)->void{if(d==1){res[0]=0;memcpy(res+1,at(1,1,0),m*4);return;}u32 size=2*d,degree=m-d;
        for(u32 i=1;i<size-2;++i)for(u32 j=0;j<i;++j){u32*dst=at(d-1,i,j);memcpy(dst,at(d,i,j),(degree+1)*4);dst[degree+1]=0;}self(self,d-1,res);
        for(u32 i=1;i<size-2;++i)for(u32 j=0;j<i;++j){u32 *a1=at(d,size-2,i),*a2=at(d,size-1,i),*b1=at(d,size-1,j),*b2=at(d,size-2,j),*dst=at(d-1,i,j);
            for(u32 k=0;k<=degree;++k){u32 value=0;for(u32 first=0;first<=k;first+=8){u64 sum=0;for(u32 l=first;l<=k&&l<first+8;++l)sum+=u64(a1[l])*b1[k-l]+u64(a2[l])*b2[k-l];value=M::add(value,sum%P);}dst[k+1]=M::add(dst[k+1],value);}
        }
        u32*p=at(d,size-1,size-2),*tmp=work.p+(d-2)*(m+1);self(self,d-1,tmp);res[d-1]=0;
        for(u32 i=d;i<=m;++i){u32 value=tmp[i];for(u32 first=d-1;first<i;first+=8){u64 sum=0;for(u32 j=first;j<i&&j<first+8;++j)sum+=u64(tmp[j])*p[i-j-1];value=M::add(value,sum%P);}res[i]=M::sub(value,res[i]);}
    };u32*result=work.p+(m-1)*(m+1);solve(solve,m,result);return result[m];
}
}
