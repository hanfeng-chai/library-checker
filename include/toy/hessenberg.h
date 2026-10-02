#pragma once
#include <toy/matrix_arbitrary.h>
#include <toy/field_dot.h>
namespace toy {
// Left transvections commute, as do their inverse column operations. Apply
// all row updates first, then one contiguous dot product per column entry.
template<u32 P=998244353>Buffer<u32> characteristic_hessenberg(u32 n,std::span<const u32>input){using M=Mod<P>;u32 stride=((n+7)&~7u)+8;Buffer<u32>a(usize(n)*stride),factors(stride);std::fill(a.p,a.p+a.n,0u);Buffer<u32*>row(n);for(u32 i=0;i<n;++i){row[i]=a.p+usize(i)*stride;memcpy(row[i],input.data()+usize(i)*n,n*4);}
    for(u32 k=0;k+2<n;++k){u32 p=k+1;while(p<n&&!row[p][k])++p;if(p==n)continue;if(p!=k+1){std::swap(row[p],row[k+1]);for(u32 i=0;i<n;++i)std::swap(row[i][p],row[i][k+1]);}u32 inverse=M::pow(row[k+1][k],P-2),length=n-k-2;std::fill(factors.p,factors.p+((length+7)&~7u),0u);
        for(u32 i=k+2;i<n;++i){u32 factor=M::mul(row[i][k],inverse);factors[i-k-2]=M::mont(factor,M::r2);if(factor)modular_row_sub(row[i],row[k+1],k+1,n,factor,P);row[i][k]=0;}
        for(u32 i=0;i<n;++i)row[i][k+1]=M::add(row[i][k+1],field_dot_padded<P>(row[i]+k+2,factors.p,length));
    }
    // Store equal-degree coefficients of successive principal polynomials
    // contiguously. Each recurrence is a dot product, not many rank-one updates.
    Buffer<u32>history(usize(n+1)*stride),coeff(stride),result(n+1);std::fill(history.p,history.p+history.n,0u);std::fill(coeff.p,coeff.p+stride,0u);result[0]=1;for(u32 i=0;i<=n;++i)history[usize(i)*stride]=M::mont(1,M::r2);
    for(u32 i=0;i<n;++i){coeff[i]=M::sub(0,row[i][i]);u32 product=1;for(u32 j=i;j--;){product=M::mul(product,row[j+1][j]);coeff[j]=M::sub(0,M::mul(product,row[j][i]));}
        for(u32 k=i+1;k--;){u32 value=field_dot_padded<P>(coeff.p+k,history.p+usize(k)*stride,i-k+1);if(k)value=M::add(value,result[k-1]);result[k]=value;history[usize(k)*stride+i+1-k]=M::mont(value,M::r2);}result[i+1]=1;
    }return result;
}
}
